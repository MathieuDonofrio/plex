#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_SHADER_PROGRAM_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_SHADER_PROGRAM_H

#include "genebits/engine/graphics/vulkan/images/vulkan_attachment.h"
#include "genebits/engine/graphics/vulkan/pipeline/vulkan_compute_pipeline.h"
#include "genebits/engine/graphics/vulkan/pipeline/vulkan_graphics_pipeline.h"
#include "genebits/engine/graphics/vulkan/pipeline/vulkan_pipeline.h"
#include "genebits/engine/graphics/vulkan/resources/vertex/vulkan_vertex_buffer.h"
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
    std::shared_ptr<VulkanGraphicsPipeline> pipeline,
    const std::string& vertex_shader_path, // NOLINT(bugprone-easily-swappable-parameters)
    const std::string& fragment_shader_path) // NOLINT(bugprone-easily-swappable-parameters)
    : device_(std::move(device)), pipeline_(std::move(pipeline))
  {
    vertex_shader_module_ = std::make_shared<VulkanShaderModule>(device_, vertex_shader_path, ShaderType::Vertex);
    fragment_shader_module_ = std::make_shared<VulkanShaderModule>(device_, fragment_shader_path, ShaderType::Fragment);
  }

  VulkanShaderProgram(std::shared_ptr<VulkanDevice> device,
    std::shared_ptr<VulkanComputePipeline> pipeline,
    const std::string& compute_shader_path)
    : device_(std::move(device)), pipeline_(std::move(pipeline))
  {
    compute_shader_module_ = std::make_shared<VulkanShaderModule>(device_, compute_shader_path, ShaderType::Compute);
    pipeline_->InitializePipeline(GetComputeShaderStageCreateInfo());
  }

  void ReadsFrom(std::shared_ptr<VulkanRenderingBuffer> rendering_buffer)
  {
    if (std::find(writes_to_.begin(), writes_to_.end(), rendering_buffer) == writes_to_.end())
    {
      LOG_ERROR("Attachment is already present in the writes list");
      return;
    }

    reads_from_.emplace_back(std::move(rendering_buffer));
  };

  void WritesTo(std::shared_ptr<VulkanRenderingBuffer> rendering_buffer)
  {
    if (std::find(reads_from_.begin(), reads_from_.end(), rendering_buffer) == reads_from_.end())
    {
      LOG_ERROR("Attachment is already present in the read list");
      return;
    }

    writes_to_.emplace_back(std::move(rendering_buffer));
  };

  void AddVertexBuffer(std::shared_ptr<VulkanVertexBuffer> vertex_buffer)
  {
    vertex_buffers_.emplace_back(std::move(vertex_buffer));
  }

  [[nodiscard]] ProgramType GetProgramType() const noexcept
  {
    return compute_shader_module_ ? ProgramType::Compute : ProgramType::Graphics;
  }

private:
  VkPipelineShaderStageCreateInfo GetFragmentShaderStageCreateInfo()
  {
    return GetShaderStageCreateInfo(fragment_shader_module_);
  }

  VkPipelineShaderStageCreateInfo GetVertexShaderStageCreateInfo()
  {
    return GetShaderStageCreateInfo(vertex_shader_module_);
  }

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
  std::shared_ptr<VulkanShaderModule> vertex_shader_module_;
  std::shared_ptr<VulkanShaderModule> fragment_shader_module_;
  std::shared_ptr<VulkanShaderModule> compute_shader_module_;

  std::vector<std::shared_ptr<VulkanRenderingBuffer>> reads_from_;
  std::vector<std::shared_ptr<VulkanRenderingBuffer>> writes_to_;

  std::vector<std::shared_ptr<VulkanVertexBuffer>> vertex_buffers_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_SHADER_PROGRAM_H
