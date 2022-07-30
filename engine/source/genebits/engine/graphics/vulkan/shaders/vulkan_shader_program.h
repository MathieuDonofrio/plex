#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_SHADER_PROGRAM_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_SHADER_PROGRAM_H

#include "genebits/engine/graphics/vulkan/pipeline/vulkan_compute_pipeline.h"
#include "genebits/engine/graphics/vulkan/pipeline/vulkan_pipeline.h"
#include "genebits/engine/graphics/vulkan/shaders/vulkan_shader_module.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include "genebits/engine/debug/logging.h"

#include <memory>
#include <string>
#include <utility>

namespace genebits::engine
{

enum class ProgramType
{
  Graphics = 0,
  Compute = 1
};

class VulkanShaderProgram
{
public:

  VulkanShaderProgram(std::shared_ptr<VulkanDevice> device,
    std::shared_ptr<VulkanComputePipeline> pipeline,
    const std::string& compute_shader_path)
    : device_(std::move(device)), pipeline_(std::move(pipeline))
  {
    compute_shader_module_ = std::make_shared<VulkanShaderModule>(device_, compute_shader_path, ShaderType::Compute);
    pipeline_->InitializePipeline(GetComputeShaderStageCreateInfo());
  }

private:

  VkPipelineShaderStageCreateInfo GetComputeShaderStageCreateInfo()
  {
    return GetShaderStageCreateInfo(compute_shader_module_);
  }

  VkPipelineShaderStageCreateInfo GetShaderStageCreateInfo(const std::shared_ptr<VulkanShaderModule>& shader_module)
  {
    VkPipelineShaderStageCreateInfo shader_stage_create_info {};
    shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_create_info.stage = shader_module->GetStage();
    shader_stage_create_info.module = shader_module->GetHandle();
    shader_stage_create_info.pName = "main";
    shader_stage_create_info.pSpecializationInfo = nullptr;
    return shader_stage_create_info;
  }

private:
  std::shared_ptr<VulkanDevice> device_;
  std::shared_ptr<VulkanPipeline> pipeline_;
  std::shared_ptr<VulkanShaderModule> compute_shader_module_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_SHADER_PROGRAM_H
