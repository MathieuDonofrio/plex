#ifndef PLEX_GRAPHICS_SHADER_H
#define PLEX_GRAPHICS_SHADER_H

#include <filesystem>
#include <string>

namespace plex::graphics
{
enum class ShaderType
{
  Vertex,
  Fragment,
  Compute,
};

enum class ShaderOptimization
{
  None,
  Size,
  Speed,
};

enum class ShaderLanguage
{
  GLSL,
  HLSL,
};

struct ShaderCompileOptions
{
  ShaderOptimization optimization;
  ShaderLanguage language;
};

class Shader
{
public:
  Shader() = default;
  virtual ~Shader() = default;

  virtual ShaderType GetType() const noexcept = 0;
};
} // namespace plex::graphics

#endif
