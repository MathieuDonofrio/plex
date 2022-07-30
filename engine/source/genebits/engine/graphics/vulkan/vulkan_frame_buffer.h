#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_FRAME_BUFFER_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_FRAME_BUFFER_H

#include "genebits/engine/graphics/vulkan/images/vulkan_color_attachment.h"
#include "genebits/engine/graphics/vulkan/images/vulkan_depth_attachment.h"

#include "genebits/engine/debug/assertion.h"

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanRenderPass;

// A frame buffer is a collection of specific memory attachments.
// The frame buffer is lazily initialized.
class VulkanFrameBuffer
{
public:
  VulkanFrameBuffer(std::shared_ptr<VulkanDevice> device,
    std::shared_ptr<VulkanRenderPass> render_pass,
   VkExtent2D extent);

  void AddColorImages(std::vector<std::shared_ptr<VulkanImage>> color_images);

  void SetDepthImage(std::shared_ptr<VulkanImage> depth_image);

  [[nodiscard]] VkFramebuffer GetFramebufferHandle();

  [[nodiscard]] std::vector<VkAttachmentDescription> GetAttachmentsDescriptions() const;

  [[nodiscard]] bool HasDepthImage() const;

private:
  VkFramebuffer frame_buffer_handle_;
  std::shared_ptr<VulkanRenderPass> render_pass_;
  std::shared_ptr<VulkanDevice> device_;

  VkExtent2D extent_;

  std::shared_ptr<VulkanImage> depth_image_;
  std::vector<std::shared_ptr<VulkanImage>> color_images_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_FRAME_BUFFER_H
