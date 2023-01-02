#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_MATERIAL_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_MATERIAL_H

#include "plex/graphics/material.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"

namespace plex::graphics
{
class VulkanMaterial : public Material
{
public:
  VulkanMaterial(VkPipeline pipeline) : pipeline_(pipeline) {}

  [[nodiscard]] VkPipeline GetPipeline() const noexcept
  {
    return pipeline_;
  }

private:
  VkPipeline pipeline_;
};

} // namespace plex::graphics

#endif
