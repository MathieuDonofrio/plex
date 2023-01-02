#include "plex/graphics/vulkan/vulkan_shader.h"

#include <fstream>

#include "plex/debug/logging.h"

namespace plex::graphics
{
namespace
{
  VkShaderStageFlagBits FromShaderStageFlags(ShaderStageFlags stage)
  {
    switch (stage)
    {
    case ShaderStageFlags::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
    case ShaderStageFlags::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
    case ShaderStageFlags::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
    case ShaderStageFlags::All: return VK_SHADER_STAGE_ALL;
    default: return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    }
  }
} // namespace

VulkanShader::VulkanShader(VkDevice device, std::vector<char> code, ShaderStageFlags stage)
  : device_(device), stage_(stage)
{

  // Create the shader module
  VkShaderModuleCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize = code.size();
  create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

  vkCreateShaderModule(device_, &create_info, nullptr, &shader_module_);
}

VulkanShader::~VulkanShader()
{
  vkDestroyShaderModule(device_, shader_module_, nullptr);
}

} // namespace plex::graphics