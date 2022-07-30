#ifndef GENEBITS_ENGINE_GRAPHICS_IMAGE_UTILS_H
#define GENEBITS_ENGINE_GRAPHICS_IMAGE_UTILS_H

#include "genebits/engine/graphics/vulkan/images/images2/vulkan_image2.h"
#include "genebits/engine/graphics/vulkan/images/images2/vulkan_memory_owning_image.h"
#include "genebits/engine/graphics/vulkan/images/images2/vulkan_non_memory_owning_image.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include <memory>
#include <utility>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanImageFactory
{
public:
  VulkanImageFactory() = delete;

  /**
   * Create a vulkan basic image create info structure with the given parameters.
   * @param extent The extent of the image.
   * @param format The format of the image.
   * @param tiling The tiling of the image.
   * @param usage The usage of the image.
   * @return The vulkan image create info structure.
   */
  static VkImageCreateInfo CreateImageCreateInfo(
    VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage)
  {
    VkImageCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    create_info.imageType = VK_IMAGE_TYPE_2D;
    create_info.extent.depth = 1;
    create_info.mipLevels = 1; // Might be good to support that in the future
    create_info.arrayLayers = 1;
    create_info.extent.width = extent.width;
    create_info.extent.height = extent.height;
    create_info.format = format;
    create_info.tiling = tiling;
    create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    create_info.usage = usage;
    create_info.samples = VK_SAMPLE_COUNT_1_BIT;
    create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    create_info.queueFamilyIndexCount = 0;
    return create_info;
  }

  /**
   * Create a basic Vulkan image view create info with the given parameters.
   * @param format The format of the image.
   * @param aspect_flags The aspect flags of the image.
   * @param image_handle The image handle to create the view for.
   * @return The vulkan image view create info structure.
   */
  static VkImageViewCreateInfo CreateImageViewCreateInfo(
    VkFormat format, VkImageAspectFlags aspect_flags, VkImage image_handle = VK_NULL_HANDLE)
  {
    VkImageViewCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    create_info.image = image_handle;
    create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    create_info.format = format;
    create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    create_info.subresourceRange.aspectMask = aspect_flags;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;
    return create_info;
  }

  /**
   * Create an image which doesn't own the memory.
   * Example: present images.
   * @param device The device to create the image on.
   * @param image_create_info The image create info.
   * @return The vulkan image.
   */
  static std::shared_ptr<VulkanImage2> CreateNonMemoryOwningImage(
    std::shared_ptr<VulkanDevice> device, const VkImageViewCreateInfo& image_view_create_info)
  {
    return std::make_shared<VulkanNonMemoryOwningImage>(std::move(device), image_view_create_info);
  }

  /**
   * Create an image which owns the memory.
   * Example: depth or color images.
   * @param device The device to create the image on.
   * @param image_create_info The image create info.
   * @param image_view_create_info The image view create info.
   * @note The image handle inside the image view create info is not required as it is set by when the image is created.
   * @return The vulkan image.
   */
  static std::shared_ptr<VulkanImage2> CreateImage(std::shared_ptr<VulkanDevice> device,
    const VkImageCreateInfo& image_create_info,
    const VkImageViewCreateInfo& image_view_create_info)
  {
    return std::make_shared<VulkanMemoryOwningImage>(std::move(device), image_create_info, image_view_create_info);
  }
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_IMAGE_UTILS_H
