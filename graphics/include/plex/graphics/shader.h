#ifndef PLEX_GRAPHICS_SHADER_H
#define PLEX_GRAPHICS_SHADER_H

#include <filesystem>
#include <string>

enum class OptimizationLevel
{
    None,
    Size,
    Speed,
};
enum class SourceLanguage
{
  GLSL,
  HLSL,
};

struct ShaderCompilationOptions {
  OptimizationLevel optimization_level = OptimizationLevel::Speed;
  SourceLanguage source_language = SourceLanguage::GLSL;
};

namespace plex::graphics
{
enum class ShaderType
{
  Vertex,
  Fragment,
  Compute,
};

class Shader
{
public:
  Shader() = default;
  virtual ~Shader() = default;

  virtual ShaderType GetType() const noexcept = 0;
};

struct ShaderCreateInfo
{
  std::filesystem::path path;
  std::string source;
  ShaderType type;
};
} // namespace plex::graphics

#endif
