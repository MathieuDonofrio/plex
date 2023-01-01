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
  frames_ = std::allocator<VulkanFrame>().allocate(swapchain_.GetImageCount());

  for (size_t i = 0; i < swapchain_.GetImageCount(); ++i)
  {
    new (frames_ + i) VulkanFrame(device_.GetHandle(), device_.GetGraphicsQueueFamilyIndex());
  }

  frame_index_ = 0;
}

VulkanRenderer::~VulkanRenderer()
{
  for (size_t i = 0; i < swapchain_.GetImageCount(); ++i)
  {
    frames_[i].~VulkanFrame();
  }

  std::allocator<VulkanFrame>().deallocate(frames_, swapchain_.GetImageCount());
}

Frame* VulkanRenderer::AquireNextFrame()
{
  // Get next frame data

  VulkanFrame* frame = frames_ + frame_index_;

  VkSemaphore image_available_semaphore = frame->GetImageAvailableSemaphore();
  VkFence fence = frame->GetFence();

  frame_index_ = (frame_index_ + 1) % GetFrameCount();

  // Wait for the fence to be signaled

  vkWaitForFences(device_.GetHandle(), 1, &fence, VK_TRUE, UINT64_MAX);
  vkResetFences(device_.GetHandle(), 1, &fence);

  // Aquire next image

  uint32_t image_index = swapchain_.AquireNextImage(image_available_semaphore);

  // Setup frame data

  frame->GetCommandPool()->Reset();

  VkImage image = swapchain_.GetImage(image_index);
  VkImageView image_view = swapchain_.GetImageView(image_index);

  frame->UpdateImageData(image, image_view, image_index);

  return frame;
}

void VulkanRenderer::Render(Frame* frame, CommandBuffer* command_buffer)
{
  // Get frame data

  auto* vulkan_frame = static_cast<VulkanFrame*>(frame);

  VkSemaphore vk_image_available_semaphore = vulkan_frame->GetImageAvailableSemaphore();
  VkSemaphore vk_render_finished_semaphore = vulkan_frame->GetRenderFinishedSemaphore();
  VkFence vk_fence = vulkan_frame->GetFence();

  // Submit command buffer

  VkCommandBuffer vk_command_buffer = static_cast<VulkanCommandBuffer*>(command_buffer)->GetHandle();

  VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

  VkSubmitInfo submit_info = {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &vk_command_buffer;
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = &vk_image_available_semaphore;
  submit_info.pWaitDstStageMask = waitStages;
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = &vk_render_finished_semaphore;

  vkQueueSubmit(device_.GetGraphicsQueue(), 1, &submit_info, vk_fence);
}

void VulkanRenderer::Present(Frame* frame)
{
  // Get frame data

  auto* vulkan_frame = static_cast<VulkanFrame*>(frame);

  uint32_t image_index = vulkan_frame->GetImageIndex();

  VkSemaphore vk_render_finished_semaphore = vulkan_frame->GetRenderFinishedSemaphore();

  // Present image

  VkSwapchainKHR vk_swapchain = swapchain_.GetHandle();

  VkPresentInfoKHR present_info = {};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = &vk_render_finished_semaphore;
  present_info.swapchainCount = 1;
  present_info.pSwapchains = &vk_swapchain;
  present_info.pImageIndices = &image_index;

  vkQueuePresentKHR(device_.GetPresentQueue(), &present_info);
}
} // namespace plex::graphics