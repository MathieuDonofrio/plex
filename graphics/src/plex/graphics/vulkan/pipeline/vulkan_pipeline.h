#ifndef PLEX_GRAPHICS_VULKAN_PIPELINE_H
#define PLEX_GRAPHICS_VULKAN_PIPELINE_H

#include <vulkan/vulkan_core.h>

namespace plex
{

class VulkanPipeline
{
public:
  virtual ~VulkanPipeline() = default;

  virtual bool InitializePipeline(const VkPipelineShaderStageCreateInfo& shader_stage_create_info) = 0;
};

} // namespace plex

#endif
