#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_DEPTH_IMAGE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_DEPTH_IMAGE_H

#include "vulkan_attachment.h"
#include "vulkan_buffer_image.h"

#include <memory>

#include "../../../../../../../../../.conan/data/vulkan-headers/1.3.216.0/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include/vulkan/vulkan_core.h"

namespace genebits::engine
{

class VulkanDepthAttachment : public VulkanBufferAttachment
{
public:
  VulkanDepthAttachment(std::shared_ptr<VulkanDevice> device, uint32_t attachment_index, VkExtent2D extent)
    : VulkanBufferAttachment(
      GetBufferAttachmentCreateInfo(device, extent), GetAttachmentCreateInfo(device, attachment_index))
  {}

private:
  static VulkanBufferAttachmentCreateInfo GetBufferAttachmentCreateInfo(
    std::shared_ptr<VulkanDevice> device, VkExtent2D extent)
  {
    VulkanBufferAttachmentCreateInfo create_info;
    create_info.format = GetDepthFormat(device);
    create_info.device = std::move(device);
    create_info.extent = extent;
    create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    create_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    create_info.properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    create_info.aspect_flags = VK_IMAGE_ASPECT_DEPTH_BIT;
    return create_info;
  }

  static VulkanAttachmentCreateInfo GetAttachmentCreateInfo(
    std::shared_ptr<VulkanDevice>& device, uint32_t attachment_index)
  {
    VkFormat format = GetDepthFormat(device);

    AttachmentOp attachment_op = { VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE };

    LayoutTransitionSequence layout_transition_sequence = { VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

    return { attachment_index, format, layout_transition_sequence, attachment_op, AttachmentType::Depth };
  }

  static VkFormat GetDepthFormat(const std::shared_ptr<VulkanDevice>& device)
  {
    return GetFormat(device,
      { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
      VK_IMAGE_TILING_OPTIMAL,
      VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
  }
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_DEPTH_IMAGE_H
