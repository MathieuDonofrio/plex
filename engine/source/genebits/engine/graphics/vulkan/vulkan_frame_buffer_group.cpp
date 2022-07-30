#include "vulkan_frame_buffer_group.h"

#include "genebits/engine/graphics/vulkan/vulkan_render_pass.h"

#include "genebits/engine/debug/assertion.h"

namespace genebits::engine
{

FrameBufferGroup::FrameBufferGroup(std::shared_ptr<VulkanDevice> device,
  std::shared_ptr<VulkanRenderPass> render_pass,
  VkExtent2D extent,
  uint32_t frame_buffer_count)
{
  frame_buffers_.reserve(frame_buffer_count);

  for (uint32_t i = 0; i < frame_buffer_count; ++i)
  {
    frame_buffers_.emplace_back(std::make_shared<VulkanFrameBuffer>(std::move(device), std::move(render_pass), extent));
  }
}

void FrameBufferGroup::AddSharedColorImages(const std::vector<std::shared_ptr<VulkanImage>>& shared_color_image)
{
  for (const auto& frame_buffer : frame_buffers_)
  {
    frame_buffer->AddColorImages(shared_color_image);
  }
}

void FrameBufferGroup::AddSharedColorImage(std::shared_ptr<VulkanImage> color_image)
{
  for (size_t i = 0; i < frame_buffers_.size(); ++i)
  {
    frame_buffers_[i]->AddColorImages({color_image});
  }
}


void FrameBufferGroup::AddColorImages(const std::vector<std::shared_ptr<VulkanImage>>& color_images)
{
  ASSERT(color_images.size() == frame_buffers_.size(),
    "color_images.size() didn't match the number of available frame buffers");

  for (size_t i = 0; i < frame_buffers_.size(); ++i)
  {
    frame_buffers_[i]->AddColorImages({ color_images[i] });
  }
}

void FrameBufferGroup::SetSharedDepthImage(std::shared_ptr<VulkanImage> shared_depth_image)
{
  for (const auto& frame_buffer : frame_buffers_)
  {
    frame_buffer->SetDepthImage(shared_depth_image);
  }
}

void FrameBufferGroup::SetDepthImages(std::vector<std::shared_ptr<VulkanImage>> depth_images)
{
  ASSERT(depth_images.size() == frame_buffers_.size(),
    "depth_images.size() didn't match the number of available frame buffers");

  for (size_t i = 0; i < frame_buffers_.size(); ++i)
  {
    frame_buffers_[i]->SetDepthImage(depth_images[i]);
  }
}

std::shared_ptr<VulkanFrameBuffer> FrameBufferGroup::GetFramebuffer(size_t frame_index)
{
  ASSERT(frame_index == frame_buffers_.size(),
    "frame_index -> " + std::to_string(frame_index) + " didn't match the number of available frame buffers -> "
      + std::to_string(frame_buffers_.size()));

  return frame_buffers_[frame_index];
}
} // namespace genebits::engine