#include "plex/graphics/vulkan/vulkan_shader.h"

namespace plex::graphics
{
namespace
{
  VkShaderStageFlagBits GetShaderStageFlagBits(ShaderType type)
  {
    switch (type)
    {
    case ShaderType::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
    case ShaderType::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
    case ShaderType::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
    default: return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    }
  }
} // namespace

VulkanShader::VulkanShader(VkDevice device, const ShaderData& shader_data, ShaderType type)
  : device_(device), type_(type)
{
  const auto& spv_binary = shader_data.shader;

  VkShaderModuleCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = spv_binary.size() * sizeof(uint32_t);
  create_info.pCode = spv_binary.data();

  vkCreateShaderModule(device, &create_info, nullptr, &shader_module_);
}

VulkanShader::~VulkanShader()
{
  vkDestroyShaderModule(device_, shader_module_, nullptr);
}

VkPipelineShaderStageCreateInfo VulkanShader::GetPipelineShaderStageCreateInfo() const noexcept
{
  VkPipelineShaderStageCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  create_info.stage = GetShaderStageFlagBits(type_);
  create_info.module = shader_module_;
  create_info.pName = "main";

  return create_info;
}
} // namespace plex::graphics