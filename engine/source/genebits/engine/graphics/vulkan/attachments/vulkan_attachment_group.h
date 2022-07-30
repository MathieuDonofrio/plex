#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_ATTACHMENT_GROUP_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_ATTACHMENT_GROUP_H

#include "genebits/engine/graphics/vulkan/attachments/vulkan_attachment.h"

#include "genebits/engine/debug/assertion.h"

#include <compare>
#include <memory>
#include <utility>
#include <vector>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanAttachmentGroup
{
public:
  VulkanAttachmentGroup() = default;

  VulkanAttachmentGroup(std::shared_ptr<VulkanAttachment> attachment) : attachments_ { std::move(attachment) } {}

  VulkanAttachmentGroup(const std::vector<std::shared_ptr<VulkanAttachment>>& attachments) : attachments_(attachments)
  {}

  VulkanAttachmentGroup(std::vector<std::shared_ptr<VulkanAttachment>>&& attachments)
    : attachments_(std::move(attachments))
  {}

  [[nodiscard]] bool IsEmpty() const noexcept
  {
    return attachments_.empty();
  }

  [[nodiscard]] bool IsType(AttachmentType attachment_type) const noexcept
  {
    ASSERT(!IsEmpty(), "Attachment group is empty.");
    return attachments_.front()->GetAttachmentType() == attachment_type;
  }

  [[nodiscard]] VkImageView GetImageView(uint32_t frame_index) const noexcept
  {
    ASSERT(!attachments_.empty(), "No attachments set");
    ASSERT(frame_index < attachments_.size(), "Frame index is out of range.");

    if (attachments_.size() == 1)
    {
      // Attachment is shared between all frames.
      return attachments_.front()->GetImage().GetImageViewHandle();
    }

    // Each attachment is unique for each frame.
    return attachments_[frame_index]->GetImage().GetImageViewHandle();
  }

  [[nodiscard]] VkAttachmentDescription GetAttachmentDescription() const noexcept
  {
    ASSERT(!attachments_.empty(), "No attachments set");
    return attachments_.front()->GetAttachmentDescription();
  }

  constexpr auto operator==(const VulkanAttachmentGroup& other) const noexcept
  {
    return group_id_ == other.group_id_;
  }

private:
  [[nodiscard]] static uint64_t GetNextGroupId() noexcept
  {
    static uint64_t next_group_id = 0;
    return next_group_id++;
  }

private:
  uint64_t group_id_ { GetNextGroupId() };
  std::vector<std::shared_ptr<VulkanAttachment>> attachments_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_ATTACHMENT_GROUP_H
