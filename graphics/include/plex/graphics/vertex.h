#ifndef PLEX_GRAPHICS_VERTEX_H
#define PLEX_GRAPHICS_VERTEX_H

#include "plex/math/vec2.h"
#include "plex/math/vec3.h"

namespace plex::graphics
{
struct Vertex
{
  float2 pos;
  float3 color;
  float3 tex; // z = texture index
};
} // namespace plex::graphics

#endif
