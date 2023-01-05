#ifndef PLEX_GRAPHICS_SHADER_COMPILER_H
#define PLEX_GRAPHICS_SHADER_COMPILER_H

#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include <shaderc/shaderc.hpp>
#include <spirv-tools/libspirv.hpp>

#include "plex/graphics/shader.h"

namespace plex::graphics
{

using ShaderSPVBinary = std::vector<uint32_t>;

struct ShaderData
{
  ShaderSPVBinary shader;
  ShaderType type;

  // TODO Reflection data
};

enum class TargetEnvironment
{
  Vulkan,
  OpenGL,
};

enum class SpirvVersion
{
  Spirv_1_0,
  Spirv_1_1,
  Spirv_1_2,
  Spirv_1_3,
  Spirv_1_4,
  Spirv_1_5,
  Spirv_1_6,
};

enum class TargetEnvironmentVersion
{
  Vulkan_1_0,
  Vulkan_1_1,
  Vulkan_1_2,
  Vulkan_1_3,
  OpenGL_4_5,
};

struct ShaderCompilerOptions
{
  TargetEnvironment target_environment;
  TargetEnvironmentVersion target_environment_version;
  SpirvVersion spirv_version;
  bool validation_enabled;
  bool generate_debug_information;
};

class ShaderCompiler
{
public:
  ShaderCompiler(const ShaderCompilerOptions& options);
  ~ShaderCompiler() = default;

  ShaderCompiler(const ShaderCompiler&) = delete;
  ShaderCompiler(ShaderCompiler&&) = delete;
  ShaderCompiler& operator=(const ShaderCompiler&) = delete;
  ShaderCompiler& operator=(ShaderCompiler&&) = delete;

  std::optional<ShaderData> Compile(const std::string& source,
    const std::filesystem::path& path,
    ShaderType type,
    ShaderCompilationOptions compile_options);

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

  void SetValidationEnabled(bool enabled)
  {
    validation_enabled_ = enabled;
  }

private:
  std::optional<shaderc::CompilationResult<uint32_t>> CompileToSpv(
    const std::string& source, const std::string& absolute_path, ShaderType type);

  spvtools::SpirvTools spirv_tools_;
  shaderc::Compiler compiler_;
  shaderc::CompileOptions options_;
  std::string error_message_;

  bool validation_enabled_ = true;
  bool valid_ = true;
};
} // namespace plex::graphics

#endif
