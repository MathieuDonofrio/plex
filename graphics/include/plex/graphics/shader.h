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
