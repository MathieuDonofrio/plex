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

  SetViewport(
    0.0f, 0.0f, static_cast<float>(context_.extent.width), static_cast<float>(context_.extent.height), 0.0f, 1.0f);

  SetScissor(0, 0, context_.extent.width, context_.extent.height);
}

void VulkanCommandBuffer::EndRenderPass()
{
  vkCmdEndRenderPass(command_buffer_);
}

void VulkanCommandBuffer::SetViewport(float x, float y, float width, float height, float min_depth, float max_depth)
{
  VkViewport viewport {};
  viewport.x = x;
  viewport.y = y;
  viewport.width = width;
  viewport.height = height;
  viewport.minDepth = min_depth;
  viewport.maxDepth = max_depth;

  vkCmdSetViewport(command_buffer_, 0, 1, &viewport);
}

void VulkanCommandBuffer::SetScissor(int32_t x, int32_t y, uint32_t width, uint32_t height)
{
  VkRect2D scissor {};
  scissor.offset = { x, y };
  scissor.extent = { width, height };

  vkCmdSetScissor(command_buffer_, 0, 1, &scissor);
}

void VulkanCommandBuffer::BindVertexBuffer(const Buffer<Vertex>& buffer)
{
  auto vk_buffer = static_cast<VkBuffer>(buffer.GetNativeHandle());
  VkDeviceSize offsets[] = { 0 };

  vkCmdBindVertexBuffers(command_buffer_, 0, 1, &vk_buffer, offsets);
}

void VulkanCommandBuffer::BindIndexBuffer(const Buffer<uint32_t>& buffer)
{
  auto vk_buffer = static_cast<VkBuffer>(buffer.GetNativeHandle());

  vkCmdBindIndexBuffer(command_buffer_, vk_buffer, 0, VK_INDEX_TYPE_UINT32);
}

void VulkanCommandBuffer::BindIndexBuffer(const Buffer<uint16_t>& buffer)
{
  auto vk_buffer = static_cast<VkBuffer>(buffer.GetNativeHandle());

  vkCmdBindIndexBuffer(command_buffer_, vk_buffer, 0, VK_INDEX_TYPE_UINT16);
}

void VulkanCommandBuffer::BindMaterial(Material* material)
{
  auto* vulkan_material = static_cast<VulkanMaterial*>(material);

  vkCmdBindPipeline(command_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan_material->GetPipeline());
}

void VulkanCommandBuffer::Draw(uint32_t vertex_count)
{
  vkCmdDraw(command_buffer_, vertex_count, 1, 0, 0);
}

void VulkanCommandBuffer::DrawIndexed(uint32_t index_count)
{
  vkCmdDrawIndexed(command_buffer_, index_count, 1, 0, 0, 0);
}

void VulkanCommandBuffer::CopyBuffer(
  pbi::Buffer src, pbi::Buffer dst, size_t src_offset, size_t dst_offset, size_t size)
{
  auto vk_src_buffer = static_cast<VkBuffer>(src->GetNativeHandle());
  auto vk_dst_buffer = static_cast<VkBuffer>(dst->GetNativeHandle());

  VkBufferCopy copy_region {};
  copy_region.srcOffset = src_offset;
  copy_region.dstOffset = dst_offset;
  copy_region.size = size;

  vkCmdCopyBuffer(command_buffer_, vk_src_buffer, vk_dst_buffer, 1, &copy_region);
}

} // namespace plex::graphics
