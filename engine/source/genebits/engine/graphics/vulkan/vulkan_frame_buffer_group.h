#ifndef GENEBITS_VULKAN_FRAME_BUFFER_GROUP_H
#define GENEBITS_VULKAN_FRAME_BUFFER_GROUP_H

#include "genebits/engine/graphics/vulkan/images/vulkan_depth_attachment.h"
#include "genebits/engine/graphics/vulkan/images/vulkan_image.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"
#include "genebits/engine/graphics/vulkan/vulkan_frame_buffer.h"
#include "genebits/engine/graphics/vulkan/vulkan_swapchain.h"

#include <memory>
#include <vector>

#include "vulkan/vulkan_core.h"

namespace genebits::engine
{

class VulkanRenderPass;

class FrameBufferGroup
{
public:
  FrameBufferGroup(std::shared_ptr<VulkanDevice> device,
    std::shared_ptr<VulkanRenderPass> render_pass,
    VkExtent2D extent,
    uint32_t frame_buffer_count);

  void AddSharedColorImages(const std::vector<std::shared_ptr<VulkanImage>>& shared_color_image);

  void AddSharedColorImage(std::shared_ptr<VulkanImage> color_image);

  void AddColorImages(const std::vector<std::shared_ptr<VulkanImage>>& color_images);

  void SetSharedDepthImage(std::shared_ptr<VulkanImage> shared_depth_image);

  void SetDepthImages(std::vector<std::shared_ptr<VulkanImage>> depth_images);

  std::shared_ptr<VulkanFrameBuffer> GetFramebuffer(size_t frame_index);
private:
  std::vector<std::shared_ptr<VulkanFrameBuffer>> frame_buffers_;
};

} // namespace genebits::engine

#endif // GENEBITS_VULKAN_FRAME_BUFFER_GROUP_H
