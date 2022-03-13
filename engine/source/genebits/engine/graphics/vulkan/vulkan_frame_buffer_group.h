#ifndef GENEBITS_VULKAN_FRAME_BUFFER_GROUP_H
#define GENEBITS_VULKAN_FRAME_BUFFER_GROUP_H

#include <memory>
#include <vector>

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/graphics/vulkan/vulkan_color_attachment.h"
#include "genebits/engine/graphics/vulkan/vulkan_depth_attachment.h"
#include "genebits/engine/graphics/vulkan/vulkan_frame_buffer.h"

#include "vulkan/vulkan_core.h"

namespace genebits::engine
{

class FrameBufferGroup
{
public:
  explicit FrameBufferGroup(uint32_t frame_count)
  {
    frame_buffers_.reserve(frame_count);
    for (int i = 0; i < frame_count; ++i)
    {
      frame_buffers_.emplace_back(std::make_shared<VulkanFrameBuffer>());
    }
  }

  void AddSharedColorAttachments(const std::vector<std::shared_ptr<VulkanColorAttachment>>& shared_color_attachments)
  {
    for (const auto& frame_buffer : frame_buffers_)
    {
      frame_buffer->AddColorAttachments(shared_color_attachments);
    }
  };

  void AddColorAttachments(const std::vector<std::shared_ptr<VulkanColorAttachment>>& color_attachments)
  {
    ASSERT(color_attachments.size() == frame_buffers_.size(),
      "color_attachments.size() didn't match the number of available frame buffers");

    for (size_t i = 0; i < frame_buffers_.size(); ++i)
    {
      frame_buffers_[i]->AddColorAttachments({ color_attachments[i] });
    }
  }

  void SetSharedDepthAttachment(std::shared_ptr<VulkanDepthAttachment> shared_depth_attachment)
  {
    for (const auto& frame_buffer : frame_buffers_)
    {
      frame_buffer->SetDepthAttachment(std::move(shared_depth_attachment));
    }
  };

  void SetDepthAttachments(std::vector<std::shared_ptr<VulkanDepthAttachment>> depth_attachments)
  {
    ASSERT(depth_attachments.size() == frame_buffers_.size(),
      "depth_attachments.size() didn't match the number of available frame buffers");

    for (size_t i = 0; i < frame_buffers_.size(); ++i)
    {
      frame_buffers_[i]->SetDepthAttachment(depth_attachments[i]);
    }
  };

  std::shared_ptr<VulkanFrameBuffer> GetFramebuffer(size_t frame_index)
  {
    ASSERT(frame_index == frame_buffers_.size(),
      ("frame_index -> {} didn't match the number of available frame buffers -> {}",
        frame_index,
        frame_buffers_.size()));
  }

private:
  // render pass handle here
  std::vector<std::shared_ptr<VulkanFrameBuffer>> frame_buffers_;
};

} // namespace genebits::engine

#endif // GENEBITS_VULKAN_FRAME_BUFFER_GROUP_H
