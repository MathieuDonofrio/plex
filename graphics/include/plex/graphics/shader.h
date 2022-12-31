#ifndef PLEX_GRAPHICS_SHADER_H
#define PLEX_GRAPHICS_SHADER_H

#include "plex/utilities/enum_flag.h"

namespace plex::graphics
{
enum class ShaderStageFlags
{
  Vertex = BitFlag(0),
  Fragment = BitFlag(1),
  Compute = BitFlag(2),
  All = Vertex | Fragment | Compute,
  None = BitMax(ShaderStageFlags)
};

DEFINE_ENUM_FLAG_OPERATORS(ShaderStageFlags);

class Shader
{
public:
  Shader() = default;
  virtual ~Shader() = default;

  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;
  Shader(Shader&&) = delete;
  Shader& operator=(Shader&&) = delete;

  virtual ShaderStageFlags GetStage() const = 0;
};
} // namespace plex::graphics

#endif
