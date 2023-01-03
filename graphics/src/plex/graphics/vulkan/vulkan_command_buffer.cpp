#include "plex/graphics/vulkan/vulkan_command_buffer.h"

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
  VkClearValue clear_value {};
  clear_value.color = { 0.0f, 0.0f, 0.0f, 1.0f };

  VkRenderPassBeginInfo render_pass_begin_info {};
  render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_begin_info.renderPass = context_.render_pass;
  render_pass_begin_info.framebuffer = context_.framebuffer;
  render_pass_begin_info.renderArea.offset = { 0, 0 };
  render_pass_begin_info.renderArea.extent = context_.extent;
  render_pass_begin_info.clearValueCount = 1;
  render_pass_begin_info.pClearValues = &clear_value;

  vkCmdBeginRenderPass(command_buffer_, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandBuffer::EndRenderPass()
{
  vkCmdEndRenderPass(command_buffer_);
}

void VulkanCommandBuffer::FirstTriangleTest(Material* material)
{
  auto* vulkan_material = static_cast<VulkanMaterial*>(material);

  vkCmdBindPipeline(command_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan_material->GetPipeline());

  VkViewport viewport {};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)context_.extent.width;
  viewport.height = (float)context_.extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(command_buffer_, 0, 1, &viewport);

  VkRect2D scissor {};
  scissor.offset = { 0, 0 };
  scissor.extent = context_.extent;
  vkCmdSetScissor(command_buffer_, 0, 1, &scissor);

  vkCmdDraw(command_buffer_, 3, 1, 0, 0);
}

} // namespace plex::graphics
