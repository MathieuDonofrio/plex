#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_SHADER_COMPILER_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_SHADER_COMPILER_H

#include <filesystem>
#include <memory>
#include <optional>
#include <set>
#include <vector>

#include <shaderc/shaderc.hpp>

#include "plex/graphics/shader.h"
#include "plex/graphics/vulkan/vulkan_shader_include_handler.h"
#include "plex/graphics/vulkan/vulkan_spv_binary.h"

namespace plex::graphics
{

class VulkanShaderCompiler
{
public:
  VulkanShaderCompiler();
  VulkanShaderCompiler(const VulkanShaderCompiler&) = delete;
  VulkanShaderCompiler(VulkanShaderCompiler&&) = delete;
  VulkanShaderCompiler& operator=(const VulkanShaderCompiler&) = delete;
  VulkanShaderCompiler& operator=(VulkanShaderCompiler&&) = delete;
  ~VulkanShaderCompiler() = default;

  [[nodiscard]] std::optional<VulkanSpvBinary> Compile(const std::filesystem::path& path, ShaderStageFlags stage);

  [[nodiscard]] bool HasError() const
  {
    return !error_message_.empty();
  }

  [[nodiscard]] const std::string& GetErrorMessage() const
  {
    return error_message_;
  }

  void ClearError()
  {
    error_message_.clear();
  }

  bool AddIncludeDirectory(const std::filesystem::path& path, bool recursive, uint32_t max_depth = 4);

  void ClearIncludePaths() { include_handler_->Clear(); }

private:
  shaderc::Compiler compiler_;
  shaderc::CompileOptions options_;
  std::string error_message_;

  std::unique_ptr<VulkanShaderIncludeHandler> include_handler_;
};

} // namespace plex::graphics

#endif