#include "plex/graphics/vulkan/vulkan_command_buffer.h"

#include "plex/graphics/vulkan/vulkan_shader.h"

namespace plex::graphics
{
void VulkanCommandBuffer::Reset()
{
  vkResetCommandBuffer(command_buffer_, 0);
}

void VulkanCommandBuffer::Begin()
{
  VkCommandBufferBeginInfo begin_info {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.pNext = nullptr;
  begin_info.pInheritanceInfo = nullptr;
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(command_buffer_, &begin_info);
}

void VulkanCommandBuffer::End()
{
  vkEndCommandBuffer(command_buffer_);
}

void VulkanCommandBuffer::BeginRenderPass()
{
  // TODO
}

void VulkanCommandBuffer::EndRenderPass()
{
  // TODO
}

void VulkanCommandBuffer::FirstTriangleTest(
  [[maybe_unused]] const std::vector<char>& vert, [[maybe_unused]] const std::vector<char>& frag)
{
  // Create Graphics Pipeline
}

} // namespace plex::graphics
