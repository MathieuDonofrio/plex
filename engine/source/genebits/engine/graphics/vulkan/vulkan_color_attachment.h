#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_COLOR_ATTACHMENT_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_COLOR_ATTACHMENT_H

#include "genebits/engine/graphics/vulkan/vulkan_attachment.h"

namespace genebits::engine
{

class VulkanColorAttachment : public VulkanAttachment
{
public:
  VulkanColorAttachment(uint32_t attachment_index,
    VkFormat format,
    VkAttachmentLoadOp load_op,
    VkAttachmentStoreOp store_op,
    VkImageLayout initial_layout,
    VkImageLayout final_layout)
    : VulkanAttachment(attachment_index,
      format,
      load_op,
      store_op,
      initial_layout,
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      final_layout)
  {}

private:
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_COLOR_ATTACHMENT_H
