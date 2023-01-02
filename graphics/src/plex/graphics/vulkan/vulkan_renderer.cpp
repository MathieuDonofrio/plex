#include "plex/graphics/vulkan/vulkan_renderer.h"

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
    swapchain_(&device_, &surface_, present_mode, width, height, image_count)
{
  ASSERT(GetFrameCount() <= 3, "Frame count must be less than or equal to 3");

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
    command_pool_create_info.queueFamilyIndex = device_.GetGraphicsQueueFamilyIndex();
    command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    vkCreateCommandPool(device_.GetHandle(), &command_pool_create_info, nullptr, &frame.command_pool);
  }

  current_frame_index_ = 0;
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

  // Update frame data

  vkResetCommandPool(device_.GetHandle(), frame.command_pool, 0);

  VkCommandBufferAllocateInfo command_buffer_allocate_info {};
  command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  command_buffer_allocate_info.commandPool = frame.command_pool;
  command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  command_buffer_allocate_info.commandBufferCount = 1;

  VkCommandBuffer command_buffer;
  vkAllocateCommandBuffers(device_.GetHandle(), &command_buffer_allocate_info, &command_buffer);

  frame.primary_command_buffer.~VulkanCommandBuffer();
  new (&frame.primary_command_buffer) VulkanCommandBuffer(command_buffer);

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
} // namespace plex::graphics