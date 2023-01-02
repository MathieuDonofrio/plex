#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_COMMAND_BUFFER_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_COMMAND_BUFFER_H

#include "plex/graphics/command_buffer.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"
#include "plex/graphics/vulkan/vulkan_device.h"

namespace plex::graphics
{
class VulkanCommandBuffer : public CommandBuffer
{
public:
  VulkanCommandBuffer() = default;

  VulkanCommandBuffer(VkCommandBuffer command_buffer) : command_buffer_(command_buffer) {}

  void Reset() override;

  void Begin() override;
  void End() override;

  void BeginRenderPass() override;
  void EndRenderPass() override;

  void FirstTriangleTest(const std::vector<char>& vert, const std::vector<char>& frag) override; // TODO remove

  [[nodiscard]] VkCommandBuffer GetHandle() const noexcept
  {
    return command_buffer_;
  }

private:
  VkCommandBuffer command_buffer_;
};
} // namespace plex::graphics

#endif
