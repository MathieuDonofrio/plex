#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_PIPELINE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_PIPELINE_H

namespace genebits::engine
{

enum class PipelineType
{
  Graphics = 0,
  Compute = 1,
};

class VulkanPipeline
{
public:

  [[nodiscard]] const PipelineType GetPipelineType() const noexcept
  {
    return pipeline_type_;
  }

  virtual bool InitializePipeline(const VkPipelineShaderStageCreateInfo& shader_stage_create_info) = 0;

protected:
  PipelineType pipeline_type_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_PIPELINE_H
