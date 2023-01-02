#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_MATERIAL_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_MATERIAL_H

#include "plex/graphics/material.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"

namespace plex::graphics
{
class VulkanMaterial : public Material
{
public:
  VulkanMaterial(VkPipelineLayout pipeline_layout, VkPipeline pipeline)
    : pipeline_layout_(pipeline_layout), pipeline_(pipeline)
  {}

  [[nodiscard]] VkPipelineLayout GetPipelineLayout() const noexcept
  {
    return pipeline_layout_;
  }

  [[nodiscard]] VkPipeline GetPipeline() const noexcept
  {
    return pipeline_;
  }

private:
  VkPipelineLayout pipeline_layout_;
  VkPipeline pipeline_;
};

} // namespace plex::graphics

#endif
