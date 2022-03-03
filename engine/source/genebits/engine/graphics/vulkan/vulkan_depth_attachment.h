#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_DEPTH_ATTACHMENT_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_DEPTH_ATTACHMENT_H

#include "genebits/engine/graphics/vulkan/vulkan_attachment.h"

namespace genebits::engine
{

class VulkanDepthAttachment : public VulkanAttachment
{
public:
  VulkanDepthAttachment(uint32_t attachment_index, VkFormat format)
    : VulkanAttachment(attachment_index,
      format,
      VK_ATTACHMENT_LOAD_OP_CLEAR,
      VK_ATTACHMENT_STORE_OP_DONT_CARE,
      VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
  {}

private:
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_DEPTH_ATTACHMENT_H
