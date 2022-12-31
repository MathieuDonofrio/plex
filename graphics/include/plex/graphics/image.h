#ifndef PLEX_GRAPHICS_IMAGE_H
#define PLEX_GRAPHICS_IMAGE_H

#include <cstdint>

#include "plex/utilities/enum_flag.h"

namespace plex::graphics
{
enum class ImageFormat
{
  RGB,
  RGBA,
  BGR,
  BGRA,
  LUMINANCE,
  LUMINANCE_ALPHA,
  UNDEFINED
};

enum class ImageUsageFlags
{
  TransferSrc = BitFlag(0),
  TransferDst = BitFlag(1),
  Sampled = BitFlag(2),
  Storage = BitFlag(3),
  ColorAttachment = BitFlag(4),
  DepthStencilAttachment = BitFlag(5),
  TransientAttachment = BitFlag(6),
  InputAttachment = BitFlag(7),
  Undefined = BitMax(ImageUsageFlags),
};

DEFINE_ENUM_FLAG_OPERATORS(ImageUsageFlags);

class Image
{
public:
  Image() = default;
  virtual ~Image() = default;

  Image(const Image&) = delete;
  Image& operator=(const Image&) = delete;
  Image(Image&&) = delete;
  Image& operator=(Image&&) = delete;

  [[nodiscard]] virtual ImageFormat GetFormat() const = 0;

  [[nodiscard]] virtual ImageUsageFlags GetImageUsageFlags() const = 0;

  [[nodiscard]] virtual int32_t GetWidth() const = 0;

  [[nodiscard]] virtual int32_t GetHeight() const = 0;
};

} // namespace plex::graphics

#endif
