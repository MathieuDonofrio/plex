#include "plex/graphics/vulkan/vulkan_renderer.h"

#include "plex/graphics/vulkan/vulkan_material.h"
#include "plex/graphics/vulkan/vulkan_shader.h"

namespace plex::graphics
{
VulkanRenderer::VulkanRenderer(VulkanCapableWindow* window,
  const std::string& application_name,
  DebugLevel debug_level,
  PresentMode present_mode,
  uint32_t width,
  uint32_t height,
  uint32_t image_count)
  : instance_(application_name, debug_level, window->GetRequiredInstanceExtensions()),
    surface_(window, instance_.GetHandle()), device_(instance_.GetHandle(), surface_.GetHandle()),
    swapchain_(&device_, &surface_, present_mode, width, height, image_count), current_frame_index_(0),
    current_image_index_(0)
{
  ASSERT(GetFrameCount() <= 3, "Frame count must be less than or equal to 3");

  // Make frame data

  for (size_t i = 0; i < GetFrameCount(); i++)
  {
    FrameData& frame = frames_[i];

    VkSemaphoreCreateInfo semaphore_create_info {};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.pNext = nullptr;
    semaphore_create_info.flags = 0;

    vkCreateSemaphore(device_.GetHandle(), &semaphore_create_info, nullptr, &frame.image_available_semaphore);
    vkCreateSemaphore(device_.GetHandle(), &semaphore_create_info, nullptr, &frame.render_finished_semaphore);

    VkFenceCreateInfo fence_create_info {};
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.pNext = nullptr;
    fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    vkCreateFence(device_.GetHandle(), &fence_create_info, nullptr, &frame.fence);

    VkCommandPoolCreateInfo command_pool_create_info {};
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_create_info.queueFamilyIndex = device_.GetGraphicsQueueFamilyIndex();

    vkCreateCommandPool(device_.GetHandle(), &command_pool_create_info, nullptr, &frame.command_pool);
  }

  // Make shared data

  VkAttachmentDescription color_attachment {};
  color_attachment.format = swapchain_.GetSurfaceFormat().format;
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_attachment_reference {};
  color_attachment_reference.attachment = 0;
  color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass {};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_reference;

  VkSubpassDependency subpass_dependency {};
  subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  subpass_dependency.dstSubpass = 0;
  subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subpass_dependency.srcAccessMask = 0;
  subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  subpass_dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo render_pass_create_info {};
  render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass_create_info.attachmentCount = 1;
  render_pass_create_info.pAttachments = &color_attachment;
  render_pass_create_info.subpassCount = 1;
  render_pass_create_info.pSubpasses = &subpass;
  render_pass_create_info.dependencyCount = 1;
  render_pass_create_info.pDependencies = &subpass_dependency;

  vkCreateRenderPass(device_.GetHandle(), &render_pass_create_info, nullptr, &render_pass_);

  swapchain_.CreateFrameBuffers(render_pass_);
}

VulkanRenderer::~VulkanRenderer()
{
  for (size_t i = 0; i < GetFrameCount(); i++)
  {
    FrameData& frame = frames_[i];

    vkWaitForFences(device_.GetHandle(), 1, &frame.fence, VK_TRUE, UINT64_MAX); // TODO is this nessesary?

    vkDestroySemaphore(device_.GetHandle(), frame.image_available_semaphore, nullptr);
    vkDestroySemaphore(device_.GetHandle(), frame.render_finished_semaphore, nullptr);
    vkDestroyFence(device_.GetHandle(), frame.fence, nullptr);
    vkDestroyCommandPool(device_.GetHandle(), frame.command_pool, nullptr);
  }

  vkDestroyRenderPass(device_.GetHandle(), render_pass_, nullptr);
}

CommandBuffer* VulkanRenderer::AquireNextFrame()
{
  // Get next frame

  FrameData& frame = frames_[current_frame_index_];

  // Wait for the fence to be signaled

  vkWaitForFences(device_.GetHandle(), 1, &frame.fence, VK_TRUE, UINT64_MAX);
  vkResetFences(device_.GetHandle(), 1, &frame.fence);

  // Aquire next image

  current_image_index_ = swapchain_.AquireNextImage(frame.image_available_semaphore);

  // Reset command pool

  vkResetCommandPool(device_.GetHandle(), frame.command_pool, 0);

  VkCommandBufferAllocateInfo command_buffer_allocate_info {};
  command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  command_buffer_allocate_info.commandPool = frame.command_pool;
  command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  command_buffer_allocate_info.commandBufferCount = 1;

  // Create primary command buffer

  VkCommandBuffer command_buffer;
  vkAllocateCommandBuffers(device_.GetHandle(), &command_buffer_allocate_info, &command_buffer);

  VulkanCommandBufferContext context;
  context.device = device_.GetHandle();
  context.render_pass = render_pass_;
  context.framebuffer = swapchain_.GetFramebuffer(current_image_index_);
  context.extent = swapchain_.GetExtent();

  frame.primary_command_buffer.~VulkanCommandBuffer();
  new (&frame.primary_command_buffer) VulkanCommandBuffer(command_buffer, context);

  return &frame.primary_command_buffer;
}

void VulkanRenderer::Render()
{
  // Get current frame

  const FrameData& frame = frames_[current_frame_index_];

  // Submit command buffer

  VkCommandBuffer vk_command_buffer = frame.primary_command_buffer.GetHandle();

  VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

  VkSubmitInfo submit_info = {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &vk_command_buffer;
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = &frame.image_available_semaphore;
  submit_info.pWaitDstStageMask = waitStages;
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = &frame.render_finished_semaphore;

  vkQueueSubmit(device_.GetGraphicsQueue(), 1, &submit_info, frame.fence);
}

void VulkanRenderer::Present()
{
  // Get current frame

  const FrameData& frame = frames_[current_frame_index_];

  // Present image

  VkSwapchainKHR vk_swapchain = swapchain_.GetHandle();

  VkPresentInfoKHR present_info = {};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = &frame.render_finished_semaphore;
  present_info.swapchainCount = 1;
  present_info.pSwapchains = &vk_swapchain;
  present_info.pImageIndices = &current_image_index_;

  vkQueuePresentKHR(device_.GetPresentQueue(), &present_info);

  // Increment frame index

  current_frame_index_ = (current_frame_index_ + 1) % GetFrameCount();
}

std::unique_ptr<Material> VulkanRenderer::CreateMaterial(const MaterialCreateInfo& create_info)
{
  // Pipeline layout

  VkPipelineLayoutCreateInfo pipeline_layout_create_info {};
  pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout_create_info.setLayoutCount = 0;
  pipeline_layout_create_info.pSetLayouts = nullptr;
  pipeline_layout_create_info.pushConstantRangeCount = 0;
  pipeline_layout_create_info.pPushConstantRanges = nullptr;

  VkPipelineLayout pipeline_layout;
  vkCreatePipelineLayout(device_.GetHandle(), &pipeline_layout_create_info, nullptr, &pipeline_layout);

  // Shaders

  auto* vertex_shader = static_cast<VulkanShader*>(create_info.vertex_shader);
  auto* fragment_shader = static_cast<VulkanShader*>(create_info.fragment_shader);

  VkPipelineShaderStageCreateInfo shader_stages[] = { vertex_shader->GetPipelineShaderStageCreateInfo(),
    fragment_shader->GetPipelineShaderStageCreateInfo() };

  // Vertex Input

  // TODO currently vertex data is hardcoded in the shader

  VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info {};
  vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_input_state_create_info.vertexBindingDescriptionCount = 0;
  vertex_input_state_create_info.pVertexBindingDescriptions = nullptr;
  vertex_input_state_create_info.vertexAttributeDescriptionCount = 0;
  vertex_input_state_create_info.pVertexAttributeDescriptions = nullptr;

  // Input Assembly

  VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info {};
  input_assembly_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly_state_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

  // Viewports & Scissors

  VkViewport viewport {};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapchain_.GetExtent().width);
  viewport.height = static_cast<float>(swapchain_.GetExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor {};
  scissor.offset = { 0, 0 };
  scissor.extent = swapchain_.GetExtent();

  VkPipelineViewportStateCreateInfo viewport_state_create_info {};
  viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state_create_info.viewportCount = 1;
  viewport_state_create_info.pViewports = &viewport;
  viewport_state_create_info.scissorCount = 1;
  viewport_state_create_info.pScissors = &scissor;

  // Rasterizer

  VkPipelineRasterizationStateCreateInfo rasterizer_create_info {};
  rasterizer_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer_create_info.depthClampEnable = VK_FALSE;
  rasterizer_create_info.rasterizerDiscardEnable = VK_FALSE;
  rasterizer_create_info.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer_create_info.lineWidth = 1.0f;
  rasterizer_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterizer_create_info.depthBiasEnable = VK_FALSE;
  rasterizer_create_info.depthBiasConstantFactor = 0.0f;
  rasterizer_create_info.depthBiasClamp = 0.0f;
  rasterizer_create_info.depthBiasSlopeFactor = 0.0f;

  // Multisampling

  VkPipelineMultisampleStateCreateInfo multisampling_create_info {};
  multisampling_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling_create_info.sampleShadingEnable = VK_FALSE;
  multisampling_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling_create_info.minSampleShading = 1.0f;
  multisampling_create_info.pSampleMask = nullptr;
  multisampling_create_info.alphaToCoverageEnable = VK_FALSE;
  multisampling_create_info.alphaToOneEnable = VK_FALSE;

  // Color Blending

  VkPipelineColorBlendAttachmentState color_blend_attachment_state {};
  color_blend_attachment_state.colorWriteMask =
    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  color_blend_attachment_state.blendEnable = VK_FALSE;
  color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
  color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;

  VkPipelineColorBlendStateCreateInfo color_blending_create_info {};
  color_blending_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blending_create_info.logicOpEnable = VK_FALSE;
  color_blending_create_info.logicOp = VK_LOGIC_OP_COPY;
  color_blending_create_info.attachmentCount = 1;
  color_blending_create_info.pAttachments = &color_blend_attachment_state;
  color_blending_create_info.blendConstants[0] = 0.0f;
  color_blending_create_info.blendConstants[1] = 0.0f;
  color_blending_create_info.blendConstants[2] = 0.0f;
  color_blending_create_info.blendConstants[3] = 0.0f;

  // Create graphics pipeline

  VkGraphicsPipelineCreateInfo pipeline_create_info {};
  pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_create_info.stageCount = 2;
  pipeline_create_info.pStages = shader_stages;
  pipeline_create_info.pVertexInputState = &vertex_input_state_create_info;
  pipeline_create_info.pInputAssemblyState = &input_assembly_state_create_info;
  pipeline_create_info.pViewportState = &viewport_state_create_info;
  pipeline_create_info.pRasterizationState = &rasterizer_create_info;
  pipeline_create_info.pMultisampleState = &multisampling_create_info;
  pipeline_create_info.pColorBlendState = &color_blending_create_info;
  pipeline_create_info.layout = pipeline_layout;
  pipeline_create_info.renderPass = render_pass_;
  pipeline_create_info.subpass = 0;
  pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
  pipeline_create_info.basePipelineIndex = -1;

  VkPipeline pipeline;
  vkCreateGraphicsPipelines(device_.GetHandle(), VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &pipeline);

  return std::make_unique<VulkanMaterial>(pipeline_layout, pipeline);
}

std::unique_ptr<Shader> VulkanRenderer::CreateShader(char* shader_code, size_t size, ShaderType type)
{
  return std::make_unique<VulkanShader>(device_.GetHandle(), shader_code, size, type);
}
} // namespace plex::graphics