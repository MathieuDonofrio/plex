#ifndef GENEBITS_ENGINE_GRPAHICS_VULKANFRAMEBUFFER_H
#define GENEBITS_ENGINE_GRPAHICS_VULKANFRAMEBUFFER_H

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/graphics/vulkan/vulkan_color_attachment.h"
#include "genebits/engine/graphics/vulkan/vulkan_depth_attachment.h"


#include "vulkan/vulkan_core.h"

namespace genebits::engine
{

class VulkanFrameBuffer
{
public:
  VulkanFrameBuffer()
    : frame_buffer_handle_(VK_NULL_HANDLE)
  {}

  void AddColorAttachments(std::vector<std::shared_ptr<VulkanColorAttachment>> color_attachments)
  {
    ASSERT(!frame_buffer_handle_, "Cannot modify attachments of framebuffer after calling GetFramebufferHandle()");
    color_attachments_.insert(color_attachments_.end(), color_attachments.begin(), color_attachments.end());
  }

  void SetDepthAttachment(std::shared_ptr<VulkanDepthAttachment> depth_attachment)
  {
    ASSERT(!frame_buffer_handle_, "Cannot modify attachments of framebuffer after calling GetFramebufferHandle()");
    depth_attachment_ = std::move(depth_attachment);
  }

  std::shared_ptr<VkFramebuffer> GetFramebufferHandle()
  {
    if (frame_buffer_handle_) return frame_buffer_handle_;

//    VkFramebufferCreateInfo fb_info = {};
//    fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//    fb_info.pNext = nullptr;
//    fb_info.renderPass = render_pass_->GetHandle();
//    fb_info.attachmentCount = 2;
//    fb_info.pAttachments = attachments;
//    fb_info.width = info.width;
//    fb_info.height = info.height;
//    fb_info.layers = 1;
  }

private:
  std::shared_ptr<VkFramebuffer> frame_buffer_handle_;

  std::shared_ptr<VulkanDepthAttachment> depth_attachment_;
  std::vector<std::shared_ptr<VulkanColorAttachment>> color_attachments_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRPAHICS_VULKANFRAMEBUFFER_H
