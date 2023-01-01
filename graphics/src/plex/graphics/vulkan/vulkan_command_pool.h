#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_COMMAND_POOL_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_COMMAND_POOL_H

#include "plex/graphics/command_pool.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"
#include "plex/graphics/vulkan/vulkan_device.h"

namespace plex::graphics
{
class VulkanCommandPool : public CommandPool
{
public:
  VulkanCommandPool() = default;

  VulkanCommandPool(VkDevice device, uint32_t queue_family_index);

  ~VulkanCommandPool() override;

  VkCommandBuffer AllocateNative() const;

  CommandBuffer* Allocate() override;

  void Free(CommandBuffer* command_buffer) override;

  void Reset() override;

  [[nodiscard]] VkCommandPool GetHandle() const noexcept
  {
    return command_pool_;
  }

private:
  VkCommandPool command_pool_;
  VkDevice device_;
};
} // namespace plex::graphics

#endif