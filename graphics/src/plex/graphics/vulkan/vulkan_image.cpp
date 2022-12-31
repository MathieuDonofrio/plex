#include "plex/graphics/vulkan/vulkan_image.h"

#include "plex/debug/logging.h"

namespace plex::graphics
{
namespace
{
  constexpr VkFormat FromImageFormat(ImageFormat format)
  {
    switch (format)
    {
    case ImageFormat::RGB: return VK_FORMAT_R8G8B8_UNORM;
    case ImageFormat::RGBA: return VK_FORMAT_R8G8B8A8_UNORM;
    case ImageFormat::BGR: return VK_FORMAT_B8G8R8_UNORM;
    case ImageFormat::BGRA: return VK_FORMAT_B8G8R8A8_UNORM;
    case ImageFormat::LUMINANCE: return VK_FORMAT_R8_UNORM;
    case ImageFormat::LUMINANCE_ALPHA: return VK_FORMAT_R8G8_UNORM;
    default: return VK_FORMAT_UNDEFINED;
    }
  }

  constexpr VkImageUsageFlags FromImageUsageFlags(ImageUsageFlags flags)
  {
    VkImageUsageFlags result = 0;
    if (static_cast<bool>(flags & ImageUsageFlags::TransferSrc)) result |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    if (static_cast<bool>(flags & ImageUsageFlags::TransferDst)) result |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    if (static_cast<bool>(flags & ImageUsageFlags::Sampled)) result |= VK_IMAGE_USAGE_SAMPLED_BIT;
    if (static_cast<bool>(flags & ImageUsageFlags::Storage)) result |= VK_IMAGE_USAGE_STORAGE_BIT;
    if (static_cast<bool>(flags & ImageUsageFlags::ColorAttachment)) result |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (static_cast<bool>(flags & ImageUsageFlags::DepthStencilAttachment))
      result |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    if (static_cast<bool>(flags & ImageUsageFlags::TransientAttachment))
      result |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
    if (static_cast<bool>(flags & ImageUsageFlags::InputAttachment)) result |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
    return result;
  }
} // namespace

VulkanImage::VulkanImage(VkDevice device, int32_t width, int32_t height, ImageFormat format, ImageUsageFlags usage)
  : device_(device), format_(format), usage_(usage), width_(width), height_(height)
{
  VkImageCreateInfo image_info = {};
  image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  image_info.imageType = VK_IMAGE_TYPE_2D;
  image_info.extent.width = width;
  image_info.extent.height = height;
  image_info.extent.depth = 1;
  image_info.mipLevels = 1;
  image_info.arrayLayers = 1;
  image_info.format = FromImageFormat(format_);
  image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
  image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  image_info.usage = FromImageUsageFlags(usage);
  image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  image_info.samples = VK_SAMPLE_COUNT_1_BIT;

  if (vkCreateImage(device_, &image_info, nullptr, &image_) != VK_SUCCESS)
  {
    LOG_ERROR("Failed to create image");
    return;
  }
}

VulkanImage::VulkanImage(
  VkImage image, VkDevice device, int32_t width, int32_t height, ImageFormat format, ImageUsageFlags usage)
  : image_(image), device_(device), format_(format), usage_(usage), width_(width), height_(height)
{}

VulkanImage::~VulkanImage()
{
  vkDestroyImage(device_, image_, nullptr);
}

} // namespace plex::graphics