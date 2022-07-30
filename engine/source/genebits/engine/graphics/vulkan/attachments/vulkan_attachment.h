#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_ATTACHMENT_2_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_ATTACHMENT_2_H

#include "genebits/engine/graphics/vulkan/attachments/vulkan_attachment_type.h"
#include "genebits/engine/graphics/vulkan/images/vulkan_image.h"

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

struct LayoutTransitionSequence
{
  VkImageLayout initial_layout;
  VkImageLayout layout;
  VkImageLayout final_layout;
};

struct AttachmentOp
{
  VkAttachmentLoadOp load_op { VK_ATTACHMENT_LOAD_OP_DONT_CARE };
  VkAttachmentStoreOp store_op { VK_ATTACHMENT_STORE_OP_DONT_CARE };
};

struct VulkanAttachmentCreateInfo
{
  uint32_t attachment_index;
  LayoutTransitionSequence layout_transition_sequence;
  AttachmentOp attachment_op;
  AttachmentOp attachment_stencil_op {}; // Optional
  AttachmentType attachment_type;

  VulkanAttachmentCreateInfo(
    uint32_t attachment_index, LayoutTransitionSequence layout_transition_sequence, AttachmentOp attachment_op,
    AttachmentType attachment_type)
    : attachment_index(attachment_index)
    , layout_transition_sequence(layout_transition_sequence)
    , attachment_op(attachment_op)
    , attachment_type(attachment_type)
  {}
};

class VulkanAttachment
{
public:
  VulkanAttachment(std::shared_ptr<VulkanImage> image, const VulkanAttachmentCreateInfo& create_info)
    : image_(std::move(image))
  {
    attachment_type_ = create_info.attachment_type;

    attachment_description_.format = image_->GetFormat();
    attachment_description_.samples = VK_SAMPLE_COUNT_1_BIT;

    const auto attachment_op = create_info.attachment_op;
    attachment_description_.loadOp = attachment_op.load_op;
    attachment_description_.storeOp = attachment_op.store_op;

    const auto attachment_stencil_op = create_info.attachment_stencil_op;
    attachment_description_.stencilLoadOp = attachment_stencil_op.load_op;
    attachment_description_.stencilStoreOp = attachment_stencil_op.store_op;

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

  [[nodiscard]] const VulkanImage& GetImage() const noexcept
  {
    return *image_;
  }

private:
  VkAttachmentDescription attachment_description_ {};
  VkAttachmentReference attachment_reference_ {};
  AttachmentType attachment_type_ { AttachmentType::None };

  std::shared_ptr<VulkanImage> image_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_ATTACHMENT_2_H
