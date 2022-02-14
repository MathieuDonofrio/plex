#ifndef GENEBITS_VULKAN_IMAGE_H
#define GENEBITS_VULKAN_IMAGE_H

#include "genebits/engine/debug/logging.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"
#include "genebits/engine/graphics/vulkan/vulkan_render_pass.h"

#include <memory>
#include <utility>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanImage
{
public:
  VulkanImage() = default;

  virtual ~VulkanImage() = default;

  // This destructor should be in a subclass --> example: TextureImage
  // (where we do own the image -> in contrast to PresentImage where the image is owned by the swap chain)
  //  {
  //    if (image_view_handle_)
  //    {
  //      vkDestroyImageView(device_->GetHandle(), image_view_handle_, nullptr);
  //      image_view_handle_ = VK_NULL_HANDLE;
  //    }
  //
  //    if (image_handle_)
  //    {
  //      vkDestroyImage(device_->GetHandle(), image_handle_, nullptr);
  //      image_handle_ = VK_NULL_HANDLE;
  //    }
  //  }

protected:
  VkImage image_handle_;
  VkImageView image_view_handle_;
  std::shared_ptr<VulkanDevice> device_;

  VulkanImage(VkImage image_handle, std::shared_ptr<VulkanDevice> device)
    : image_handle_(image_handle), device_(std::move(device)), image_view_handle_(VK_NULL_HANDLE)
  {}
};

} // namespace genebits::engine

#endif // GENEBITS_VULKAN_IMAGE_H
