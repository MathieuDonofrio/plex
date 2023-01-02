#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_SHADER_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_SHADER_H

#include "plex/graphics/shader.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"

namespace plex::graphics
{
class VulkanShader : public Shader
{
public:
  VulkanShader(VkDevice device, char* code, size_t size, ShaderType type);

  ~VulkanShader() override;

  [[nodiscard]] VkPipelineShaderStageCreateInfo GetPipelineShaderStageCreateInfo() const noexcept;

  [[nodiscard]] VkShaderModule GetShaderModule() const noexcept
  {
    return shader_module_;
  }

  [[nodiscard]] ShaderType GetType() const noexcept override
  {
    return type_;
  }

private:
  VkDevice device_;
  VkShaderModule shader_module_;
  ShaderType type_;
};
} // namespace plex::graphics

#endif
