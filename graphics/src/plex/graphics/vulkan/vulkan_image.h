#ifndef PLEX_GRAPHICS_VULKAN_IMAGE_H
#define PLEX_GRAPHICS_VULKAN_IMAGE_H

#include "plex/graphics/image.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"

namespace plex::graphics
{
class VulkanImage : public Image
{
public:
  VulkanImage(VkDevice device, int32_t width, int32_t height, ImageFormat format, ImageUsageFlags usage);

  VulkanImage(VkImage image, VkDevice device, int32_t width, int32_t height, ImageFormat format, ImageUsageFlags usage);

  ~VulkanImage();

  [[nodiscard]] ImageFormat GetFormat() const override
  {
    return format_;
  }

  [[nodiscard]] ImageUsageFlags GetImageUsageFlags() const override
  {
    return usage_;
  }

  [[nodiscard]] int32_t GetWidth() const override
  {
    return width_;
  }

  [[nodiscard]] int32_t GetHeight() const override
  {
    return height_;
  }

  [[nodiscard]] VkImage GetHandle() const noexcept
  {
    return image_;
  }

private:
  VkImage image_;
  VkDevice device_;
  ImageFormat format_;
  ImageUsageFlags usage_;
  int32_t width_;
  int32_t height_;
};

} // namespace plex::graphics
#endif
