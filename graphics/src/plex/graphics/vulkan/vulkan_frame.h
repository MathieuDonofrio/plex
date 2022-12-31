#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_FRAME_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_FRAME_H

#include "plex/graphics/frame.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"
#include "plex/graphics/vulkan/vulkan_command_buffer.h"
#include "plex/graphics/vulkan/vulkan_command_pool.h"
#include "plex/graphics/vulkan/vulkan_fence.h"
#include "plex/graphics/vulkan/vulkan_image.h"
#include "plex/graphics/vulkan/vulkan_semaphore.h"

namespace plex::graphics
{
struct VulkanFrameCreateInfo
{
  VulkanDevice* device;
  VkImage image;
  uint32_t width;
  uint32_t height;
  ImageFormat format;
  ImageUsageFlags usage;
  int queue_family_index;
};

class VulkanFrame : public Frame
{
public:
  VulkanFrame(const VulkanFrameCreateInfo& create_info);

  // clang-format off

  virtual Image* GetImage() { return &image_; }

  virtual Semaphore* GetImageAvailableSemaphore() { return &image_available_semaphore_; }
  virtual Semaphore* GetRenderFinishedSemaphore() { return &render_finished_semaphore_; }

  virtual Fence* GetFence() { return &fence_; }

  virtual CommandPool* GetCommandPool() { return &command_pool_; }
  virtual CommandBuffer* GetMainCommandBuffer() { return &main_command_buffer_; }

  // clang-format on

private:
  VulkanImage image_;

  VulkanSemaphore image_available_semaphore_;
  VulkanSemaphore render_finished_semaphore_;

  VulkanFence fence_;

  VulkanCommandPool command_pool_;
  VulkanCommandBuffer main_command_buffer_;
};
} // namespace plex::graphics

#endif
