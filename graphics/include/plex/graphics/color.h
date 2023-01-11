#ifndef PLEX_GRAPHICS_COLOR_H
#define PLEX_GRAPHICS_COLOR_H

#include "plex/math/funcs_common.h"
#include "plex/math/vec3.h"
#include "plex/math/vec4.h"

namespace plex::graphics
{

template<typename T, size_t Size>
struct Color : public Vec<T, Size>
{
  Color(const Vec<T, Size>& vec) : Vec<T, Size>(vec) {}
};

namespace details
{
  constexpr Vec<float, 3> FromHSV(float h, float s, float v) noexcept // NOLINT(bugprone-easily-swappable-parameters)
  {
    float r, g, b;
    float i;
    float f, p, q, t;
    if (s == 0)
    {
      // achromatic (grey)
      r = g = b = v;
      return { r, g, b };
    }
    h /= 60; // sector 0 to 5
    i = Floor(h);
    f = h - i; // factorial part of h
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));
    switch (static_cast<int>(i))
    {
    case 0: r = v, g = t, b = p; break;
    case 1: r = q, g = v, b = p; break;
    case 2: r = p, g = v, b = t; break;
    case 3: r = p, g = q, b = v; break;
    case 4: r = t, g = p, b = v; break;
    default: r = v, g = p, b = q; break;
    }
    return { r, g, b };
  }

} // namespace details

struct [[maybe_unused]] RGBint : public Color<uint8_t, 3>
{
  [[nodiscard]] static Color<uint8_t, 3> FromHSV(
    float h, float s, float v) noexcept // NOLINT(bugprone-easily-swappable-parameters)
  {
    const auto color = details::FromHSV(h, s, v);
    const auto r = static_cast<uint8_t>(color.r);
    const auto g = static_cast<uint8_t>(color.g);
    const auto b = static_cast<uint8_t>(color.b);
    return { { r, g, b } };
  }
};

struct [[maybe_unused]] RGBfloat : public Color<float, 3>
{
  [[nodiscard]] static Color<float, 3> FromHSV(
    float h, float s, float v) noexcept // NOLINT(bugprone-easily-swappable-parameters)
  {
    return details::FromHSV(h, s, v);
  }
};

struct [[maybe_unused]] RGBAint : public Color<uint8_t, 4>
{
  [[nodiscard]] static Color<uint8_t, 4> FromHSV(
    float h, float s, float v, float a = 1.0f) noexcept // NOLINT(bugprone-easily-swappable-parameters)
  {
    const auto color = details::FromHSV(h, s, v);
    const auto r = static_cast<uint8_t>(color.r);
    const auto g = static_cast<uint8_t>(color.g);
    const auto b = static_cast<uint8_t>(color.b);
    const auto alpha = static_cast<uint8_t>(a * 255.0f);
    return { { r, g, b, alpha } };
  }
};

struct [[maybe_unused]] RGBAfloat : public Color<float, 4>
{
  [[nodiscard]] static Color<float, 4> FromHSV(
    float h, float s, float v, float a = 1.0f) noexcept // NOLINT(bugprone-easily-swappable-parameters)
  {
    const auto color = details::FromHSV(h, s, v);
    return { { color.r, color.g, color.b, a } };
  }
};

} // namespace plex::graphics

#endif // PLEX_GRAPHICS_COLOR_H
