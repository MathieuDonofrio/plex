#include "plex/graphics/vulkan/vulkan_command_pool.h"

#include "plex/graphics/vulkan/vulkan_command_buffer.h"

namespace plex::graphics
{
VulkanCommandPool::VulkanCommandPool(VkDevice device, uint32_t queue_family_index) : device_(device)
{
  VkCommandPoolCreateInfo command_pool_create_info {};
  command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  command_pool_create_info.queueFamilyIndex = queue_family_index;
  command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

  vkCreateCommandPool(device_, &command_pool_create_info, nullptr, &command_pool_);
}

VulkanCommandPool::~VulkanCommandPool()
{
  vkDestroyCommandPool(device_, command_pool_, nullptr);
}

VkCommandBuffer VulkanCommandPool::AllocateNative() const
{
  VkCommandBufferAllocateInfo command_buffer_allocate_info {};
  command_buffer_allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  command_buffer_allocate_info.commandPool = command_pool_;
  command_buffer_allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  command_buffer_allocate_info.commandBufferCount = 1;

  VkCommandBuffer command_buffer;
  vkAllocateCommandBuffers(device_, &command_buffer_allocate_info, &command_buffer);

  return command_buffer;
}

CommandBuffer* VulkanCommandPool::Allocate()
{
  return new VulkanCommandBuffer(AllocateNative());
}

void VulkanCommandPool::Free(CommandBuffer* command_buffer)
{
  VkCommandBuffer vk_command_buffer = static_cast<VulkanCommandBuffer*>(command_buffer)->GetHandle();

  vkFreeCommandBuffers(device_, command_pool_, 1, &vk_command_buffer);
}

void VulkanCommandPool::Reset()
{
  vkResetCommandPool(device_, command_pool_, 0);
}
} // namespace plex::graphics