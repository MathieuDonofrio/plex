#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_FRAME_BUFFER_2_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_FRAME_BUFFER_2_H

#include "genebits/engine/graphics/vulkan/attachments/vulkan_attachment_2.h"
#include "genebits/engine/graphics/vulkan/attachments/vulkan_attachment_type.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/debug/logging.h"
#include "genebits/engine/graphics/vulkan/attachments/vulkan_attachment_group.h"

#include <memory>
#include <utility>
#include <vector>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanFrameBuffer2
{
public:
  VulkanFrameBuffer2(std::shared_ptr<VulkanDevice> device, VkExtent2D extent, uint32_t frame_count)
    : device_(std::move(device)), extent_(extent), frame_count_(frame_count)
  {}

  void AddColorAttachmentGroup(const VulkanAttachmentGroup& color_attachment_group)
  {
    ASSERT(color_attachment_group.IsType(AttachmentType::Color), "Color attachment is not a color attachment.");
    color_attachments_groups_.emplace_back(color_attachment_group);
    ++attachment_count_;
  }

  void AddDepthAttachmentGroup(const VulkanAttachmentGroup& depth_attachment_group)
  {
    if (!depth_attachment_group_.IsEmpty()) LOG_WARN("Depth attachments are already set.");
    ASSERT(depth_attachment_group.IsType(AttachmentType::Depth), "Depth attachment is not a depth attachment.");
    depth_attachment_group_ = depth_attachment_group;
    ++attachment_count_;
  }

  [[nodiscard]] bool HasDepthAttachment() const noexcept
  {
    return !depth_attachment_group_.IsEmpty();
  }

  [[nodiscard]] bool HasColorAttachmentGroup(const VulkanAttachmentGroup& color_attachment_group) const noexcept
  {
    return std::find(color_attachments_groups_.begin(), color_attachments_groups_.end(), color_attachment_group)
           != color_attachments_groups_.end();
  }

  [[nodiscard]] VkFramebuffer GetHandle(uint32_t frame_index) const
  {
    ASSERT(!frame_buffer_handles_.empty(), "Frame buffer handles are not initialized.");
    ASSERT(frame_index < frame_count_, "Frame index is out of range.");
    return frame_buffer_handles_[frame_index];
  }

  [[nodiscard]] std::vector<VkAttachmentDescription> GetAttachmentDescriptions() const
  {
    std::vector<VkAttachmentDescription> attachment_descriptions(attachment_count_);

    for (const auto& color_attachment_group : color_attachments_groups_)
    {
      attachment_descriptions.emplace_back(color_attachment_group.GetAttachmentDescription());
    }

    if (!depth_attachment_group_.IsEmpty())
    {
      attachment_descriptions.emplace_back(depth_attachment_group_.GetAttachmentDescription());
    }

    return attachment_descriptions;
  }

  void InitializeFrameBuffers(VkRenderPass render_pass_handle)
  {
    ASSERT(frame_buffer_handles_.empty(), "Frame buffer handles are already initialized.");
    frame_buffer_handles_.resize(frame_count_);

    std::vector<VkImageView> image_views(attachment_count_);
    for (uint32_t i = 0; i < attachment_count_; ++i)
    {
      for (const auto& color_attachment_group : color_attachments_groups_)
      {
        image_views[i] = color_attachment_group.GetImageView(i);
      }

      if (!depth_attachment_group_.IsEmpty())
      {
        image_views[i] = depth_attachment_group_.GetImageView(i);
      }

      VkFramebufferCreateInfo frame_buffer_create_info = {};
      frame_buffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      frame_buffer_create_info.renderPass = render_pass_handle;
      frame_buffer_create_info.attachmentCount = attachment_count_;
      frame_buffer_create_info.pAttachments = image_views.data();
      frame_buffer_create_info.width = extent_.width;
      frame_buffer_create_info.height = extent_.height;
      frame_buffer_create_info.layers = 1;

      auto frame_buffer = &frame_buffer_handles_[i];
      if (vkCreateFramebuffer(device_->GetHandle(), &frame_buffer_create_info, nullptr, frame_buffer) != VK_SUCCESS)
      {
        LOG_ERROR("Failed to create frame buffer.");
      }
      LOG_TRACE("Frame buffer created.");
    }
  }

private:
  std::vector<VkFramebuffer> frame_buffer_handles_;
  std::shared_ptr<VulkanDevice> device_;

  uint32_t attachment_count_ = 0;
  uint32_t frame_count_;
  VkExtent2D extent_;

  std::vector<VulkanAttachmentGroup> color_attachments_groups_;
  VulkanAttachmentGroup depth_attachment_group_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_FRAME_BUFFER_2_H
