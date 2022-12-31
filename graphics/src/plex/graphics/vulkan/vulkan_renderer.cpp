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
{}

VulkanRenderer::~VulkanRenderer() {}

Frame* VulkanRenderer::AquireNextFrame()
{
  VulkanFrame* frame = &frames_[frame_index_];
}

void VulkanRenderer::Render(CommandBuffer* command_buffer)
{
  VulkanFrame& frame = frames_[frame_index_];

  VkSemaphore vk_image_available_semaphore =
    static_cast<VulkanSemaphore*>(frame.GetImageAvailableSemaphore())->GetHandle();
  VkSemaphore vk_render_finished_semaphore =
    static_cast<VulkanSemaphore*>(frame.GetRenderFinishedSemaphore())->GetHandle();
  VkFence fence = static_cast<VulkanFence*>(frame.GetFence())->GetHandle();

  VkCommandBuffer vk_command_buffer = static_cast<VulkanCommandBuffer*>(command_buffer)->GetHandle();

  VkSubmitInfo submit_info = {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers = &vk_command_buffer;
  submit_info.waitSemaphoreCount = 1;
  submit_info.pWaitSemaphores = &vk_image_available_semaphore;
  // submit_info.pWaitDstStageMask = &wait_stages; // TODO ?
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores = &vk_render_finished_semaphore;

  vkQueueSubmit(device_.GetGraphicsQueue(), 1, &submit_info, fence);

  frame_index_ = (frame_index_ + 1) % swapchain_.GetImageCount();
}

void VulkanRenderer::Present()
{
  VulkanFrame& frame = frames_[frame_index_];

  VkSemaphore vk_render_finished_semaphore =
    static_cast<VulkanSemaphore*>(frame.GetRenderFinishedSemaphore())->GetHandle();

  VkSwapchainKHR vk_swapchain = swapchain_.GetHandle();

  VkPresentInfoKHR present_info = {};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  present_info.waitSemaphoreCount = 1;
  present_info.pWaitSemaphores = &vk_render_finished_semaphore;
  present_info.swapchainCount = 1;
  present_info.pSwapchains = &vk_swapchain;
  present_info.pImageIndices = &frame_index_;

  vkQueuePresentKHR(device_.GetPresentQueue(), &present_info);
}
} // namespace plex::graphics