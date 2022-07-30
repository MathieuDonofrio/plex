#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_PRESENT_IMAGE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_PRESENT_IMAGE_H

#include "genebits/engine/debug/logging.h"
#include "vulkan_attachment.h"
#include "vulkan_image.h"

#include "../../../../../../../../../.conan/data/vulkan-headers/1.3.216.0/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include/vulkan/vulkan_core.h"

namespace genebits::engine
{

class VulkanPresentAttachment : public VulkanImage
{
  friend class VulkanSwapchain;

public:
  VulkanPresentAttachment(const VulkanPresentAttachment&) = delete;
  VulkanPresentAttachment& operator=(const VulkanPresentAttachment&) = delete;
  VulkanPresentAttachment(VulkanPresentAttachment&&) = default;
  VulkanPresentAttachment& operator=(VulkanPresentAttachment&&) = default;

  ~VulkanPresentAttachment() override
  {
    if (image_view_handle_)
    {
      vkDestroyImageView(device_->GetHandle(), image_view_handle_, nullptr);
      image_view_handle_ = VK_NULL_HANDLE;
      LOG_TRACE("Vulkan image view destroyed");
    }
  }

private:
  explicit VulkanPresentAttachment(const VulkanImageCreateInfo& image_create_info, uint32_t attachment_index)
    : VulkanImage(image_create_info, GetAttachmentCreateInfo(image_create_info, attachment_index)) {};

  static std::shared_ptr<VulkanPresentAttachment> Create(const VulkanImageCreateInfo& create_info, uint32_t attachment_index)
  {
    return std::shared_ptr<VulkanPresentAttachment>(new VulkanPresentAttachment(create_info, attachment_index));
  }

  static VulkanAttachmentCreateInfo GetAttachmentCreateInfo(
    const VulkanImageCreateInfo& create_info, uint32_t attachment_index)
  {
    AttachmentOp attachment_op = {
      VK_ATTACHMENT_LOAD_OP_CLEAR, // Clear buffer when starting render pass // TODO LOAD_OP_CLEAR requires a clear
                                   // value see: https://vulkan-tutorial.com/Depth_buffering#page_Framebuffer
      VK_ATTACHMENT_STORE_OP_STORE // Keep results after render pass
    };

    LayoutTransitionSequence layout_transition_sequence = {
      VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    return { attachment_index, create_info.format, layout_transition_sequence, attachment_op, AttachmentType::Present };
  }
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_PRESENT_IMAGE_H
