#ifndef PLEX_GRAPHICS_MATERIAL_H
#define PLEX_GRAPHICS_MATERIAL_H

#include "plex/graphics/shader.h"

namespace plex::graphics
{
class Material
{
public:
  Material() = default;
  virtual ~Material() = default;

  Material(const Material&) = delete;
  Material& operator=(const Material&) = delete;
  Material(Material&&) = delete;
  Material& operator=(Material&&) = delete;
};

struct MaterialCreateInfo
{
  Shader* vertex_shader;
  Shader* fragment_shader;
};
} // namespace plex::graphics

#endif
