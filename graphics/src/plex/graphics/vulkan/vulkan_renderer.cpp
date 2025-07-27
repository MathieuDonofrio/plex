#include "plex/graphics/vulkan/vulkan_renderer.h"

#include "plex/debug/logging.h"
#include "plex/graphics/vulkan/vulkan_buffer.h"
#include "plex/graphics/vulkan/vulkan_material.h"
#include "plex/graphics/vulkan/vulkan_shader.h"

namespace plex::graphics
{

VulkanRenderer::VulkanRenderer(const RendererCreateInfo& create_info)
  : // Window
    window_(create_info.window),
    // Instance
    instance_(create_info.application_name,
      create_info.debug_level,
      dynamic_cast<VulkanCapableWindow*>(window_)->GetRequiredInstanceExtensions()),
    // Surface
    surface_(dynamic_cast<VulkanCapableWindow*>(window_), instance_.GetHandle()),
    // Device
    device_(instance_.GetHandle(), surface_.GetHandle()),
    // Swapchain
    swapchain_(window_,
      &device_,
      &surface_,
      create_info.present_mode,
      create_info.buffering_mode == BufferingMode::Double ? 2 : 3),
    current_frame_index_(0), current_image_index_(0), shader_compiler_({ TargetEnvironment::Vulkan,
                                                        TargetEnvironmentVersion::Vulkan_1_3,
                                                        SpirvVersion::Spirv_1_6,
                                                        create_info.shader_validation_enabled,
                                                        create_info.shader_debug_info_enabled })
{
  ASSERT(GetFrameCount() <= 3, "Frame count must be less than or equal to 3");

  WindowEventCallback<WindowFramebufferResizeEvent> callback;
  callback.Bind<VulkanRenderer, &VulkanRenderer::WindowFramebufferResizeCallback>(this);
  window_->AddWindowFramebufferResizeEventCallback(callback);

  // Make frame data

  for (size_t i = 0; i < GetFrameCount(); i++)
  {
    FrameLocalData& frame = frames_[i];

    // Create semaphores

    VkSemaphoreCreateInfo semaphore_create_info {};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphore_create_info.pNext = nullptr;
    semaphore_create_info.flags = 0;

    vkCreateSemaphore(device_.GetHandle(), &semaphore_create_info, nullptr, &frame.image_available_semaphore);
    vkCreateSemaphore(device_.GetHandle(), &semaphore_create_info, nullptr, &frame.render_finished_semaphore);

    // Create CPU fence

    VkFenceCreateInfo fence_create_info {};
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.pNext = nullptr;
    fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    vkCreateFence(device_.GetHandle(), &fence_create_info, nullptr, &frame.fence);

    // Create command pool

    VkCommandPoolCreateInfo command_pool_create_info {};
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_create_info.queueFamilyIndex = device_.GetGraphicsQueueFamilyIndex();

    vkCreateCommandPool(device_.GetHandle(), &command_pool_create_info, nullptr, &frame.command_pool);

    VkCommandBufferAllocateInfo command_buffer_allocate_info {};
    command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.commandPool = frame.command_pool;
    command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandBufferCount = 1;

    // Create primary command buffer

    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(device_.GetHandle(), &command_buffer_allocate_info, &command_buffer);
    new (&frame.primary_command_buffer) VulkanCommandBuffer(command_buffer);
  }

  // Make global data

  // Make render pass

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

  // Init framebuffers

  swapchain_.InitFramebuffers(render_pass_);

  // Create fences

  VkFenceCreateInfo fence_create_info {};
  fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fence_create_info.pNext = nullptr;
  fence_create_info.flags = 0;

  vkCreateFence(device_.GetHandle(), &fence_create_info, nullptr, &submit_immediate_fence_);

  // Create command pool

  VkCommandPoolCreateInfo command_pool_create_info {};
  command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  command_pool_create_info.queueFamilyIndex = device_.GetGraphicsQueueFamilyIndex();

  vkCreateCommandPool(device_.GetHandle(), &command_pool_create_info, nullptr, &command_pool_);

  // Create command buffer

  VkCommandBufferAllocateInfo command_buffer_allocate_info {};
  command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  command_buffer_allocate_info.commandPool = command_pool_;
  command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  command_buffer_allocate_info.commandBufferCount = 1;

  VkCommandBuffer command_buffer;
  vkAllocateCommandBuffers(device_.GetHandle(), &command_buffer_allocate_info, &command_buffer);
  new (&submit_immediate_command_buffer_) VulkanCommandBuffer(command_buffer);
}

VulkanRenderer::~VulkanRenderer()
{
  WindowEventCallback<WindowFramebufferResizeEvent> callback;
  callback.Bind<VulkanRenderer, &VulkanRenderer::WindowFramebufferResizeCallback>(this);
  window_->RemoveWindowFramebufferResizeEventCallback(callback);

  for (size_t i = 0; i < GetFrameCount(); i++)
  {
    FrameLocalData& frame = frames_[i];

    vkWaitForFences(device_.GetHandle(), 1, &frame.fence, VK_TRUE, UINT64_MAX); // TODO is this nessesary?

    vkDestroySemaphore(device_.GetHandle(), frame.image_available_semaphore, nullptr);
    vkDestroySemaphore(device_.GetHandle(), frame.render_finished_semaphore, nullptr);
    vkDestroyFence(device_.GetHandle(), frame.fence, nullptr);
    vkDestroyCommandPool(device_.GetHandle(), frame.command_pool, nullptr);
  }

  vkDestroyRenderPass(device_.GetHandle(), render_pass_, nullptr);
  vkDestroyFence(device_.GetHandle(), submit_immediate_fence_, nullptr);
  vkDestroyCommandPool(device_.GetHandle(), command_pool_, nullptr);
}

CommandBuffer* VulkanRenderer::AcquireNextFrame()
{
  // Get the next frame

  FrameLocalData& frame = frames_[current_frame_index_];

  // Aquire the next image

  vkWaitForFences(device_.GetHandle(), 1, &frame.fence, VK_TRUE, UINT64_MAX);

  auto result = swapchain_.AquireNextImage(frame.image_available_semaphore);

  if (result == UINT32_MAX) return nullptr;

  vkResetFences(device_.GetHandle(), 1, &frame.fence);

  current_image_index_ = result;

  // Reset the command pool (Resets all buffers)

  vkResetCommandPool(device_.GetHandle(), frame.command_pool, 0);

  // Set Context

  VulkanCommandBufferContext context;
  context.device = device_.GetHandle();
  context.render_pass = render_pass_;
  context.framebuffer = swapchain_.GetFramebuffer(current_image_index_);
  context.extent = swapchain_.GetExtent();

  frame.primary_command_buffer.SetContext(context);

  return &frame.primary_command_buffer;
}

void VulkanRenderer::Render()
{
  // Get current frame

  const FrameLocalData& frame = frames_[current_frame_index_];

  // Submit command buffer

  VkCommandBuffer vk_command_buffer = frame.primary_command_buffer.GetHandle();

  VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

  VkSubmitInfo submit_info = {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.pNext = nullptr;
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = &frame.image_available_semaphore;
  submit_info.pWaitDstStageMask = waitStages;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &vk_command_buffer;
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = &frame.render_finished_semaphore;

  vkQueueSubmit(device_.GetGraphicsQueue(), 1, &submit_info, frame.fence);
}

void VulkanRenderer::Present()
{
  // Get current frame

  const FrameLocalData& frame = frames_[current_frame_index_];

  // Present image

  swapchain_.Present(current_image_index_, device_.GetPresentQueue(), frame.render_finished_semaphore);

  // Increment frame index

  current_frame_index_ = (current_frame_index_ + 1) % GetFrameCount();
}

void VulkanRenderer::SubmitImmediate(std::function<void(CommandBuffer*)> func)
{
  func(&submit_immediate_command_buffer_);

  VkCommandBuffer vk_command_buffer = submit_immediate_command_buffer_.GetHandle();

  VkSubmitInfo submit_info = {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.pNext = nullptr;
  submit_info.waitSemaphoreCount = 0;
  submit_info.pWaitSemaphores = nullptr;
  submit_info.pWaitDstStageMask = nullptr;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &vk_command_buffer;
  submit_info.signalSemaphoreCount = 0;
  submit_info.pSignalSemaphores = nullptr;

  vkQueueSubmit(device_.GetGraphicsQueue(), 1, &submit_info, submit_immediate_fence_);

  vkWaitForFences(device_.GetHandle(), 1, &submit_immediate_fence_, VK_TRUE, UINT64_MAX);
  vkResetFences(device_.GetHandle(), 1, &submit_immediate_fence_);

  vkResetCommandPool(device_.GetHandle(), command_pool_, 0);
}

void VulkanRenderer::WaitIdle()
{
  vkDeviceWaitIdle(device_.GetHandle());
}

std::unique_ptr<Material> VulkanRenderer::CreateMaterial(const MaterialCreateInfo& create_info)
{
  // Shaders

  auto* vertex_shader = static_cast<VulkanShader*>(create_info.vertex_shader);
  auto* fragment_shader = static_cast<VulkanShader*>(create_info.fragment_shader);

  VkPipelineShaderStageCreateInfo shader_stages[] = { vertex_shader->GetPipelineShaderStageCreateInfo(),
    fragment_shader->GetPipelineShaderStageCreateInfo() };

  // Vertex Input

  VkVertexInputBindingDescription bindingDescription {};
  bindingDescription.binding = 0;
  bindingDescription.stride = sizeof(Vertex);
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

  std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions {};

  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, pos);

  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, color);

  attributeDescriptions[2].binding = 0;
  attributeDescriptions[2].location = 2;
  attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[2].offset = offsetof(Vertex, tex);

  VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info {};
  vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_input_state_create_info.vertexBindingDescriptionCount = 1;
  vertex_input_state_create_info.pVertexBindingDescriptions = &bindingDescription;
  vertex_input_state_create_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
  vertex_input_state_create_info.pVertexAttributeDescriptions = attributeDescriptions.data();

  // Input Assembly

  VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info {};
  input_assembly_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly_state_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

  // Viewports

  VkPipelineViewportStateCreateInfo viewport_state_create_info {};
  viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state_create_info.viewportCount = 1;
  viewport_state_create_info.scissorCount = 1;

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

  // Dynamic State

  VkDynamicState dynamic_states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

  VkPipelineDynamicStateCreateInfo dynamic_state_create_info {};
  dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state_create_info.dynamicStateCount = 2;
  dynamic_state_create_info.pDynamicStates = dynamic_states;

  // Create pipeline layout

  VkPipelineLayoutCreateInfo pipeline_layout_create_info {};
  pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout_create_info.setLayoutCount = 0;
  pipeline_layout_create_info.pSetLayouts = nullptr;
  pipeline_layout_create_info.pushConstantRangeCount = 0;
  pipeline_layout_create_info.pPushConstantRanges = nullptr;

  VkPipelineLayout pipeline_layout;
  vkCreatePipelineLayout(device_.GetHandle(), &pipeline_layout_create_info, nullptr, &pipeline_layout);

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
  pipeline_create_info.pDynamicState = &dynamic_state_create_info;
  pipeline_create_info.layout = pipeline_layout;
  pipeline_create_info.renderPass = render_pass_;
  pipeline_create_info.subpass = 0;
  pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
  pipeline_create_info.basePipelineIndex = -1;

  VkPipeline pipeline;
  vkCreateGraphicsPipelines(device_.GetHandle(), VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &pipeline);

  return std::make_unique<VulkanMaterial>(pipeline_layout, pipeline);
}

std::unique_ptr<Shader> VulkanRenderer::CreateShader(
  const std::string& source, const std::filesystem::path& source_path, ShaderType type, ShaderCompileOptions options)
{
  auto spv_binary = shader_compiler_.Compile(source, source_path, type, options);

  if (!spv_binary)
  {
    LOG_ERROR("Failed to compile shader: {}, reason: {}", source_path.string(), shader_compiler_.GetErrorMessage());
    return nullptr;
  }

  if (spv_binary->shader.empty())
  {
    LOG_ERROR("Failed to compile shader: {}, reason: Compiled shader binary is empty", source_path.string());
    return nullptr;
  }

  return std::make_unique<VulkanShader>(device_.GetHandle(), *spv_binary, type);
}

pbi::Buffer VulkanRenderer::CreateBuffer(size_t size, BufferUsageFlags buffer_usage_flags, MemoryUsage memory_usage)
{
  return device_.CreateBuffer(size, buffer_usage_flags, memory_usage);
}

void VulkanRenderer::WindowFramebufferResizeCallback(const WindowFramebufferResizeEvent&)
{
  swapchain_.FlagFramebufferResized();
}
} // namespace plex::graphics