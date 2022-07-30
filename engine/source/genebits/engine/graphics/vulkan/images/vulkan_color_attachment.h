#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_COLOR_IMAGE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_COLOR_IMAGE_H

#include "vulkan_attachment.h"
#include "vulkan_buffer_image.h"

namespace genebits::engine
{

class VulkanColorAttachment : public VulkanBufferAttachment
{
public:
  VulkanColorAttachment(std::shared_ptr<VulkanDevice> device, uint32_t attachment_index, VkExtent2D extent)
    : VulkanBufferAttachment(
      GetBufferAttachmentCreateInfo(device, extent), GetAttachmentCreateInfo(device, attachment_index))
  {}

private:
  static VulkanBufferAttachmentCreateInfo GetBufferAttachmentCreateInfo(
    std::shared_ptr<VulkanDevice> device, VkExtent2D extent)
  {
    VulkanBufferAttachmentCreateInfo create_info;
    create_info.format = GetColorFormat(device);
    create_info.device = std::move(device);
    create_info.extent = extent;
    create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    create_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    create_info.properties =
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT; // Might change to SHARED_MEMORY_BIT depending on the use case
    create_info.aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT;
    return create_info;
  }

  static VulkanAttachmentCreateInfo GetAttachmentCreateInfo(
    std::shared_ptr<VulkanDevice>& device, uint32_t attachment_index)
  {
    VkFormat format = GetColorFormat(device);

    AttachmentOp attachment_op = { VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE };

    LayoutTransitionSequence layout_transition_sequence = {
      VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };

    return { attachment_index, format, layout_transition_sequence, attachment_op, AttachmentType::Color };
  }

  static VkFormat GetColorFormat(const std::shared_ptr<VulkanDevice>& device)
  {
    return GetFormat(
      device, { VK_FORMAT_R8G8B8A8_SRGB }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT);
  }
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_COLOR_IMAGE_H
