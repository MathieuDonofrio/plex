#include "genebits/engine/graphics/vulkan/vulkan_frame_buffer.h"

#include <utility>

#include "genebits/engine/graphics/vulkan/vulkan_render_pass.h"

namespace genebits::engine
{

VulkanFrameBuffer::VulkanFrameBuffer(
  std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanRenderPass> render_pass, VkExtent2D extent)
  : device_(std::move(device)), render_pass_(std::move(render_pass)), frame_buffer_handle_(VK_NULL_HANDLE),
    extent_(extent)
{}

void VulkanFrameBuffer::AddColorImages(std::vector<std::shared_ptr<VulkanImage>> color_images)
{
  ASSERT(!frame_buffer_handle_, "Cannot modify attachments of framebuffer after calling GetFramebufferHandle()");
  for (const auto& color_image : color_images)
  {
    bool is_color_attachment = (color_image->GetAttachmentType() & AttachmentType::Color) != AttachmentType::None;
    ASSERT(is_color_attachment, "Image is not a color attachment");
  }
  color_images_.insert(color_images_.end(), color_images.begin(), color_images.end());
}

void VulkanFrameBuffer::SetDepthImage(std::shared_ptr<VulkanImage> depth_image)
{
  ASSERT(!frame_buffer_handle_, "Cannot modify attachments of framebuffer after calling GetFramebufferHandle()");
  bool is_depth_attachment = (depth_image->GetAttachmentType() & AttachmentType::Depth) != AttachmentType::None;
  ASSERT(is_depth_attachment, "Image is not a depth attachment");
  depth_image_ = std::move(depth_image);
}

VkFramebuffer VulkanFrameBuffer::GetFramebufferHandle()
{
  if (frame_buffer_handle_) return frame_buffer_handle_;

  std::vector<VkImageView> image_views;
  for (auto& image : color_images_)
  {
    image_views.emplace_back(image->GetImageViewHandle());
  }

  if (depth_image_)
  {
    image_views.emplace_back(depth_image_->GetImageViewHandle());
  }

  VkFramebufferCreateInfo framebuffer_create_info = {};
  framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  framebuffer_create_info.pNext = nullptr;
  framebuffer_create_info.renderPass = render_pass_->GetHandle();
  framebuffer_create_info.attachmentCount = static_cast<uint32_t>(image_views.size());
  framebuffer_create_info.pAttachments = image_views.data();
  framebuffer_create_info.width = extent_.width;
  framebuffer_create_info.height = extent_.height;
  framebuffer_create_info.layers = 1;

  if (vkCreateFramebuffer(device_->GetHandle(), &framebuffer_create_info, nullptr, &frame_buffer_handle_) != VK_SUCCESS)
  {
    LOG_ERROR("Failed to create vulcan frame buffer");
    return VK_NULL_HANDLE;
  }

  return frame_buffer_handle_;
}

std::vector<VkAttachmentDescription> VulkanFrameBuffer::GetAttachmentsDescriptions() const
{
  std::vector<VkAttachmentDescription> attachments_descriptions;
  attachments_descriptions.reserve(color_images_.size());

  for (const auto& color_image : color_images_)
  {
    attachments_descriptions.emplace_back(color_image->GetAttachmentDescription());
  }

  if (depth_image_)
  {
    attachments_descriptions.emplace_back(depth_image_->GetAttachmentDescription());
  }

  return attachments_descriptions;
}

bool VulkanFrameBuffer::HasDepthImage() const
{
  return depth_image_ != nullptr;
}

} // namespace genebits::engine