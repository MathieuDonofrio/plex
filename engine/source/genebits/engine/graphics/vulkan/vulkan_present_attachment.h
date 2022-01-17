#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_PRESENT_ATTACHMENT_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_PRESENT_ATTACHMENT_H

#include "genebits/engine/graphics/vulkan/vulkan_attachment.h"

namespace genebits::engine
{

class VulkanPresentAttachment : public VulkanAttachment
{
public:
  VulkanPresentAttachment(
    uint32_t attachment_index, VkFormat format)
    : VulkanAttachment(attachment_index,
      format,
      VK_ATTACHMENT_LOAD_OP_CLEAR, // Clear buffer when starting render pass
      VK_ATTACHMENT_STORE_OP_STORE, // Keep results after render pass
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
  {}

private:
};

} // namespace genebits::engine
#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_PRESENT_ATTACHMENT_H
