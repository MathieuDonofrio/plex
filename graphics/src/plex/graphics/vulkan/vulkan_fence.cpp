#include "plex/graphics/vulkan/vulkan_fence.h"

namespace plex::graphics
{
VulkanFence::VulkanFence(VkDevice device) : device_(device)
{
  VkFenceCreateInfo fence_create_info {};
  fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fence_create_info.pNext = nullptr;
  fence_create_info.flags = 0;

  vkCreateFence(device_, &fence_create_info, nullptr, &fence_);
}

VulkanFence::~VulkanFence()
{
  vkDestroyFence(device_, fence_, nullptr);
}

void VulkanFence::Reset()
{
  vkResetFences(device_, 1, &fence_);
}

void VulkanFence::Wait()
{
  vkWaitForFences(device_, 1, &fence_, VK_TRUE, UINT64_MAX);
}

} // namespace plex::graphics
