#include "plex/graphics/vulkan/vulkan_shader.h"

#include <fstream>

#include "plex/debug/logging.h"

namespace plex::graphics
{
namespace
{
  std::vector<char> LoadShaderCodeFromFile(const std::string& filename)
  {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) [[unlikely]]
    {
      LOG_ERROR("Failed to open shader file: {}", filename);
      return {};
    }

    std::streamsize fileSize = file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
  }

  VkShaderStageFlagBits FromShaderStageFlags(ShaderStageFlags stage)
  {
    switch (stage)
    {
    case ShaderStageFlags::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
    case ShaderStageFlags::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
    case ShaderStageFlags::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
    case ShaderStageFlags::All: return VK_SHADER_STAGE_ALL;
    default:
    {
      LOG_ERROR("Unknown shader stage: {}", static_cast<int>(stage));
      return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    }
    }
  }
} // namespace

VulkanShader::VulkanShader(VkDevice device, const std::string& filename, ShaderStageFlags stage)
  : device_(device), stage_(stage)
{
  // Load the shader code from the file
  std::vector<char> code = LoadShaderCodeFromFile(filename);

  // Create the shader module
  VkShaderModuleCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = code.size();
  create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

  if (vkCreateShaderModule(device_, &create_info, nullptr, &shader_module_) != VK_SUCCESS)
  {
    LOG_ERROR("Failed to create shader module: {}", filename);
    return;
  }
}

VulkanShader::~VulkanShader()
{
  vkDestroyShaderModule(device_, shader_module_, nullptr);
}

VkPipelineShaderStageCreateInfo VulkanShader::GetPipelineShaderStageCreateInfo() const
{
  VkPipelineShaderStageCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  create_info.stage = FromShaderStageFlags(stage_);
  create_info.module = shader_module_;
  create_info.pName = "main";

  return create_info;
}

} // namespace plex::graphics