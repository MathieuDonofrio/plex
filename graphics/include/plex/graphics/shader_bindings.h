#ifndef PLEX_GRAPHICS_SHADER_BINDINGS_H
#define PLEX_GRAPHICS_SHADER_BINDINGS_H

#include <cstdint>

#include "plex/graphics/buffer.h"
#include "plex/graphics/image.h"

namespace plex::graphics
{
class ShaderBindings
{
public:
  ShaderBindings() = default;
  virtual ~ShaderBindings() = default;

  ShaderBindings(const ShaderBindings&) = delete;
  ShaderBindings& operator=(const ShaderBindings&) = delete;
  ShaderBindings(ShaderBindings&&) = delete;
  ShaderBindings& operator=(ShaderBindings&&) = delete;

  // TODO Bindings
  // UniformBuffer
  // StorageBuffer
  // SampledImage
  // StorageImage
  // InputAttachment
  // PushConstant
};

} // namespace plex::graphics

#endif
