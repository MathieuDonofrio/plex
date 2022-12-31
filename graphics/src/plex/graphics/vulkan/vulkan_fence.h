#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_FENCE_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_FENCE_H

#include "plex/graphics/fence.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"
#include "plex/graphics/vulkan/vulkan_device.h"

namespace plex::graphics
{
class VulkanFence : public Fence
{
public:
  VulkanFence(VkDevice device);

  ~VulkanFence() override;

  void Reset() override;

  void Wait() override;

  VkFence GetHandle() const noexcept
  {
    return fence_;
  }

private:
  VkFence fence_;
  VkDevice device_;
};

} // namespace plex::graphics

#endif