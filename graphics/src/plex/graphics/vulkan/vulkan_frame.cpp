#include "plex/graphics/vulkan/vulkan_frame.h"

namespace plex::graphics
{

VulkanFrame::VulkanFrame(VkDevice device, int queue_family_index)
  : device_(device), command_pool_(device, queue_family_index)
{
  VkSemaphoreCreateInfo semaphore_create_info {};
  semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  semaphore_create_info.pNext = nullptr;
  semaphore_create_info.flags = 0;

  vkCreateSemaphore(device_, &semaphore_create_info, nullptr, &image_available_semaphore_);
  vkCreateSemaphore(device_, &semaphore_create_info, nullptr, &render_finished_semaphore_);

  VkFenceCreateInfo fence_create_info {};
  fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fence_create_info.pNext = nullptr;
  fence_create_info.flags = 0;

  vkCreateFence(device_, &fence_create_info, nullptr, &fence_);
}

VulkanFrame::~VulkanFrame()
{
  vkDestroySemaphore(device_, image_available_semaphore_, nullptr);
  vkDestroySemaphore(device_, render_finished_semaphore_, nullptr);
  vkDestroyFence(device_, fence_, nullptr);
}
} // namespace plex::graphics