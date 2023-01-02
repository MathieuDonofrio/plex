#ifndef PLEX_GRAPHICS_SHADER_H
#define PLEX_GRAPHICS_SHADER_H

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
} // namespace plex::graphics

#endif
