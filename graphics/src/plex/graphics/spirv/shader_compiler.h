#ifndef PLEX_GRAPHICS_SHADER_COMPILER_H
#define PLEX_GRAPHICS_SHADER_COMPILER_H

#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <optional>
#include <string>

#include "plex/graphics/shader.h"

namespace plex::graphics
{
struct ShaderSPVBinary
{
  const char* data;
  size_t size;
};

struct ShaderData
{
  ShaderSPVBinary shader;
  ShaderType type;

  // TODO Reflection data
};

class ShaderCompiler
{
public:
  ShaderCompiler() = default;
  ~ShaderCompiler() = default;

  ShaderCompiler(const ShaderCompiler&) = delete;
  ShaderCompiler(ShaderCompiler&&) = default;
  ShaderCompiler& operator=(const ShaderCompiler&) = delete;
  ShaderCompiler& operator=(ShaderCompiler&&) = default;

  std::optional<ShaderData> Compile(const std::filesystem::path& path, const std::string& source, ShaderType type)
  {
    // TODO
    return std::nullopt;
  }
};
} // namespace plex::graphics

#endif
