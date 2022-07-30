#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_PIPELINE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_PIPELINE_H

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanPipeline
{
public:
  virtual bool InitializePipeline(const VkPipelineShaderStageCreateInfo& shader_stage_create_info) = 0;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_PIPELINE_H
