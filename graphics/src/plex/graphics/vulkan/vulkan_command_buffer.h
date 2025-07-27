#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_COMMAND_BUFFER_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_COMMAND_BUFFER_H

#include "plex/graphics/command_buffer.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"
#include "plex/graphics/vulkan/vulkan_device.h"
#include "plex/graphics/vulkan/vulkan_material.h"

namespace plex::graphics
{
struct VulkanCommandBufferContext
{
  VkDevice device;
  VkRenderPass render_pass;
  VkFramebuffer framebuffer;
  VkExtent2D extent;
};

class VulkanCommandBuffer : public CommandBuffer
{
public:
  VulkanCommandBuffer() = default;

  VulkanCommandBuffer(VkCommandBuffer command_buffer) : command_buffer_(command_buffer) {}

  void SetContext(const VulkanCommandBufferContext& context) noexcept
  {
    context_ = context;
  }

  void Reset() override;

  void Begin() override;
  void End() override;

  void BeginRenderPass() override;
  void EndRenderPass() override;

  void SetViewport(float x, float y, float width, float height, float min_depth, float max_depth) override;
  void SetScissor(int32_t x, int32_t y, uint32_t width, uint32_t height) override;

  void BindVertexBuffer(const Buffer<Vertex>& buffer) override;

  void BindIndexBuffer(const Buffer<uint32_t>& buffer) override;
  void BindIndexBuffer(const Buffer<uint16_t>& buffer) override;

  void BindMaterial(Material* material) override;

  void Draw(uint32_t vertex_count) override;
  void DrawIndexed(uint32_t index_count) override;

  [[nodiscard]] VkCommandBuffer GetHandle() const noexcept
  {
    return command_buffer_;
  }

private:
  void CopyBuffer(pbi::Buffer src, pbi::Buffer dst, size_t src_offset, size_t dst_offset, size_t size) override;

private:
  VkCommandBuffer command_buffer_;

  VulkanCommandBufferContext context_;
};
} // namespace plex::graphics

#endif
