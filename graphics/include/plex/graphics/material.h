#ifndef PLEX_GRAPHICS_MATERIAL_H
#define PLEX_GRAPHICS_MATERIAL_H

#include <vector>

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
  std::vector<char> vertex_shader_code;
  std::vector<char> fragment_shader_code;
};
} // namespace plex::graphics

#endif
