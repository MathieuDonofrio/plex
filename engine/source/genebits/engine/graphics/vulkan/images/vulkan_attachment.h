#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_ATTACHMENT_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_ATTACHMENT_H

#include "genebits/engine/graphics/vulkan/attachments/vulkan_attachment_type.h"

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{
// TODO instead of using lazy initialization, use a friend factory to call Initialize() (make_attachment)

/*
  A rendering buffer is a pixel local read or write buffer used between pipeline stages
 */
class VulkanRenderingBuffer
{
public:
  [[nodiscard]] AttachmentType GetAttachmentType() const noexcept
  {
    return attachment_type_;
  }

  [[nodiscard]] const VkAttachmentReference& GetAttachmentReference() const noexcept
  {
    return attachment_reference_;
  }

  [[nodiscard]] const VkAttachmentDescription& GetAttachmentDescription() const noexcept
  {
    return attachment_description_;
  }

protected:
  struct LayoutTransitionSequence
  {
    VkImageLayout initial_layout;
    VkImageLayout layout;
    VkImageLayout final_layout;
  };

  struct AttachmentOp
  {
    VkAttachmentLoadOp load_op;
    VkAttachmentStoreOp store_op;
  };

  struct VulkanAttachmentCreateInfo
  {
    uint32_t attachment_index;
    VkFormat format;
    LayoutTransitionSequence layout_transition_sequence;
    AttachmentOp attachment_op;
    AttachmentType attachment_type;
  };

  explicit VulkanRenderingBuffer(const VulkanAttachmentCreateInfo& create_info)
  {
    attachment_type_ = create_info.attachment_type;

    attachment_description_.format = create_info.format;
    attachment_description_.samples = VK_SAMPLE_COUNT_1_BIT;

    const auto attachment_op = create_info.attachment_op;
    attachment_description_.loadOp = attachment_op.load_op;
    attachment_description_.storeOp = attachment_op.store_op;
    // Stencil not used for now
    attachment_description_.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment_description_.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // Defines the initial layout (Let's vulkan know if there is a need to convert to another layout when calling the
    // subpass for the first time
    const auto layout_transition_sequence = create_info.layout_transition_sequence;
    attachment_description_.initialLayout = layout_transition_sequence.initial_layout;
    // Defines what layout is needed at the after this subpass
    attachment_description_.finalLayout = layout_transition_sequence.final_layout;

    // Defines the layout that is needed during the subpass
    attachment_reference_.layout = layout_transition_sequence.layout;

    attachment_reference_.attachment = create_info.attachment_index;
  }

private:
  VkAttachmentDescription attachment_description_ {};
  VkAttachmentReference attachment_reference_ {};
  AttachmentType attachment_type_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_ATTACHMENT_H
