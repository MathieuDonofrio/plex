#include "plex/graphics/vulkan/vulkan_semaphore.h"

namespace plex::graphics
{
VulkanSemaphore::VulkanSemaphore(VkDevice device) : device_(device)
{
  VkSemaphoreCreateInfo semaphore_create_info {};
  semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  semaphore_create_info.pNext = nullptr;
  semaphore_create_info.flags = 0;

  vkCreateSemaphore(device_, &semaphore_create_info, nullptr, &semaphore_);
}

VulkanSemaphore::~VulkanSemaphore()
{
  vkDestroySemaphore(device_, semaphore_, nullptr);
}

void VulkanSemaphore::Wait()
{
  VkSemaphoreWaitInfo wait_info {};
  wait_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_WAIT_INFO;
  wait_info.pNext = nullptr;
  wait_info.flags = 0;
  wait_info.semaphoreCount = 1;
  wait_info.pSemaphores = &semaphore_;
  wait_info.pValues = nullptr;

  vkWaitSemaphores(device_, &wait_info, UINT64_MAX);
}

} // namespace plex::graphics