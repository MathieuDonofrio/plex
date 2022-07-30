#ifndef GENEBITS_ENGINE_GRAPHICS_IMAGE_MANAGER_H
#define GENEBITS_ENGINE_GRAPHICS_IMAGE_MANAGER_H

#include "genebits/engine/graphics/vulkan/resources/vulkan_image_with_sampler.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"
#include "vulkan_format_util.h"

#include <memory>
#include <utility>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class Image;
class Sampler;
class SampledImage;

class TextureManager
{
  static constexpr uint32_t cMaxImages = 8192; // Arbitrary number :P
public:
  static void Create(std::shared_ptr<VulkanDevice> device)
  {
    if (instance_ == nullptr) instance_.reset(new TextureManager(std::move(device)));
  }

  static std::shared_ptr<Image> CreateTexture(VkExtent2D, VkFormat )
  {
    return nullptr; // Not implemented yet
  }

  static std::shared_ptr<VulkanImageWithSampler> CreateSampledTexture(VkExtent2D extent, VkFormat format)
  {
    VulkanImageWithSamplerCreateInfo image_with_sampler_create_info;
    image_with_sampler_create_info.vulkan_device = instance_->device_;
    image_with_sampler_create_info.binding_frequency = BindingFrequency::Auto;
    image_with_sampler_create_info.shader_stage_flags = VK_SHADER_STAGE_FRAGMENT_BIT; // Only fragment for now
    image_with_sampler_create_info.size = CalculateImageSize(extent, format);
  }

private:
  TextureManager(std::shared_ptr<VulkanDevice> device) : device_(std::move(device)) {}

  static constexpr VkDeviceSize CalculateImageSize(VkExtent2D extent2D, VkFormat format) noexcept
  {
    return static_cast<VkDeviceSize>(extent2D.width * extent2D.height) * GetFormatSize(format);
  }

private:
  static std::unique_ptr<TextureManager> instance_;
  std::shared_ptr<VulkanDevice> device_;

  struct CreateImageInfo
  {
    VkExtent2D extent;
    VkFormat format;
  };
  std::vector<CreateImageInfo> create_infos_;
  std::vector<std::shared_ptr<VulkanImageWithSampler>> sampled_images_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_IMAGE_MANAGER_H
