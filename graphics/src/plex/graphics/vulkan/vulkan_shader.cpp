#include "plex/graphics/vulkan/vulkan_shader.h"

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

VulkanShader::VulkanShader(VkDevice device, const VulkanSpvBinary& spv_binary, ShaderStageFlags stage)
  : device_(device), stage_(stage)
{
  // Create the shader module
  VkShaderModuleCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = spv_binary.GetSize() * sizeof(uint32_t);
  create_info.pCode = reinterpret_cast<const uint32_t*>(spv_binary.GetData());

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