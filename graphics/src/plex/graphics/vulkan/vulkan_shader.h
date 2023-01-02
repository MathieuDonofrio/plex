#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_SHADER_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_SHADER_H

#include <string>
#include <vector>

#include "plex/graphics/shader.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"
#include "plex/graphics/vulkan/vulkan_device.h"

namespace plex::graphics
{
class VulkanShader : public Shader
{
public:
  // TODO make this accept code not a file. This class should not have the responsibility of loading files.

  VulkanShader(VkDevice device, std::vector<char> code, ShaderStageFlags stage);

  ~VulkanShader() override;

  [[nodiscard]] ShaderStageFlags GetStage() const override
  {
    return stage_;
  }

  [[nodiscard]] VkShaderModule GetHandle() const noexcept
  {
    return shader_module_;
  }

private:
  VkShaderModule shader_module_;
  VkDevice device_;
  ShaderStageFlags stage_;
};
} // namespace plex::graphics

#endif
