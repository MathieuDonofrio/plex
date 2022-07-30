#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_SHADER_MODULE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_SHADER_MODULE_H

#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include "genebits/engine/debug/logging.h"

#include <fstream>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

enum class ShaderType
{
  Vertex = 0,
  Fragment = 1,
  Compute = 2
};

class VulkanShaderModule
{
  friend class VulkanShaderProgram;

public:
  VulkanShaderModule(std::shared_ptr<VulkanDevice> device, const std::string& shader_path, ShaderType stage)
    : device_(std::move(device))
  {
    stage_ = GetStage(stage);

    shader_module_ = CreateShaderModule(shader_path);

    pipeline_shader_stage_create_info_.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pipeline_shader_stage_create_info_.stage = stage_;
    pipeline_shader_stage_create_info_.module = shader_module_;
  }

  ~VulkanShaderModule()
  {
    if (shader_module_ != VK_NULL_HANDLE)
    {
      vkDestroyShaderModule(device_->GetHandle(), shader_module_, nullptr);
    }
  }

private:
  [[nodiscard]] VkShaderModule GetHandle() const
  {
    return shader_module_;
  }

  [[nodiscard]] VkShaderStageFlagBits GetStage() const
  {
    return stage_;
  }

  VkShaderModule CreateShaderModule(const std::string& shader_path)
  {
    std::ifstream shader_file(shader_path, std::ios::ate | std::ios::binary);
    if (!shader_file.is_open())
    {
      LOG_ERROR("Failed to open shader file: {}", shader_path);
      return VK_NULL_HANDLE;
    }

    std::streamsize file_size = shader_file.tellg();
    std::vector<char> shader_code(file_size);
    shader_file.seekg(0);
    shader_file.read(shader_code.data(), file_size);
    shader_file.close();

    VkShaderModuleCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = shader_code.size();
    create_info.pCode = reinterpret_cast<const uint32_t*>(shader_code.data());

    VkShaderModule shader_module;
    if (vkCreateShaderModule(device_->GetHandle(), &create_info, nullptr, &shader_module) != VK_SUCCESS)
    {
      LOG_ERROR("Failed to create shader module: {}", shader_path);
      return VK_NULL_HANDLE;
    }

    return shader_module;
  }

  VkShaderStageFlagBits GetStage(ShaderType stage)
  {
    switch (stage)
    {
    case ShaderType::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
    case ShaderType::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
    case ShaderType::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
    default: return VK_SHADER_STAGE_VERTEX_BIT;
    }
  }

private:
  VkShaderModule shader_module_ { VK_NULL_HANDLE };
  std::shared_ptr<VulkanDevice> device_;
  VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info_ {};
  VkShaderStageFlagBits stage_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_SHADER_MODULE_H
