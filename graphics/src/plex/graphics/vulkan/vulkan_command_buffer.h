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

  VulkanCommandBuffer(VkCommandBuffer command_buffer, VulkanCommandBufferContext context)
    : command_buffer_(command_buffer), context_(context)
  {}

  void Reset() override;

  void Begin() override;
  void End() override;

  void BeginRenderPass() override;
  void EndRenderPass() override;

  void FirstTriangleTest(Material* material) override; // TODO remove

  [[nodiscard]] VkCommandBuffer GetHandle() const noexcept
  {
    return command_buffer_;
  }

private:
  VkCommandBuffer command_buffer_;

  VulkanCommandBufferContext context_;
};
} // namespace plex::graphics

#endif
