#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_FRAME_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_FRAME_H

#include "plex/graphics/frame.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"
#include "plex/graphics/vulkan/vulkan_command_buffer.h"
#include "plex/graphics/vulkan/vulkan_command_pool.h"
#include "plex/graphics/vulkan/vulkan_image.h"

namespace plex::graphics
{
class VulkanFrame : public Frame
{
public:
  VulkanFrame(VkDevice device, int queue_family_index);

  ~VulkanFrame() override;

  CommandPool* GetCommandPool() override
  {
    return &command_pool_;
  }

  void UpdateImageData(VkImage image, VkImageView image_view, uint32_t image_index) noexcept
  {
    image_ = image;
    image_view_ = image_view;
    image_index_ = image_index;
  }

  // clang-format off

  [[nodiscard]] VkSemaphore GetImageAvailableSemaphore() const noexcept { return image_available_semaphore_; }
  [[nodiscard]] VkSemaphore GetRenderFinishedSemaphore() const noexcept { return render_finished_semaphore_; }

  [[nodiscard]] VkFence GetFence() const noexcept { return fence_; }

  [[nodiscard]] VkImage GetImage() const noexcept { return image_; }
  [[nodiscard]] VkImageView GetImageView() const noexcept { return image_view_; }
  [[nodiscard]] uint32_t GetImageIndex() const noexcept { return image_index_; }

  // clang-format on

private:
  VkDevice device_;

  VkSemaphore image_available_semaphore_;
  VkSemaphore render_finished_semaphore_;

  VkFence fence_;

  VulkanCommandPool command_pool_;

  VkImage image_;
  VkImageView image_view_;
  uint32_t image_index_;
};
} // namespace plex::graphics

#endif
