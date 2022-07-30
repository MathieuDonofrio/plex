#include "genebits/engine/graphics/vulkan/vulkan_render_pass.h"

#include "genebits/engine/graphics/vulkan/vulkan_frame_buffer.h"
#include "genebits/engine/graphics/vulkan/vulkan_frame_buffer_group.h"

#include <string>
#include <utility>

namespace genebits::engine
{

uint64_t VulkanRenderPass::next_attachment_handle_ = 0;
uint64_t VulkanRenderPass::next_subpass_handle_ = 0;

using AttachmentHandle = VulkanRenderPass::AttachmentHandle;
using SubPassHandle = VulkanRenderPass::SubPassHandle;

struct DependencyInfo
{
  //  const SubPassHandle src_sub_pass;
  //  const SubPassHandle dst_sub_pass;
  //  const ImageHandle attachment;
  //  enum class AccessType
  //  {
  //    Read,
  //    Write,
  //    Undefined
  //  };
  //  AccessType src_access_type { AccessType::Undefined };
  //  AccessType dst_access_type { AccessType::Undefined };
};

VulkanRenderPass::VulkanRenderPass(std::shared_ptr<VulkanDevice> device) : device_(std::move(device))
{
  // TODO
  //  from the docs:
  //  Care should be taken to avoid a data race here; if any subpasses access attachments with overlapping memory
  //  locations, and one of those accesses is a write, a subpass dependency needs to be included between them.
  if (!Initialize())
  {
    LOG_ERROR("Failed to initialize Vulkan Render Pass");
  }
}

// TODO:
//  When adding a new image, add it to a list. Then when compiling, create the appropriate framebuffers (multiple when
//  MultipleImage)

bool VulkanRenderPass::Initialize()
{

  //  const auto attachments_descriptions = frame_buffer_->GetAttachmentsDescriptions();
  //
  //  const auto sub_pass_descriptions = GetSubPassDescriptions();
  //
  //  VkRenderPassCreateInfo render_pass_create_info = {};
  //  render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  //  render_pass_create_info.attachmentCount = static_cast<uint32_t>(attachments_descriptions.size());
  //  render_pass_create_info.pAttachments = attachments_descriptions.data();
  //  render_pass_create_info.subpassCount = static_cast<uint32_t>(sub_pass_descriptions.size());
  //  render_pass_create_info.pSubpasses = sub_pass_descriptions.data();
  //
  //  if (vkCreateRenderPass(device_->GetHandle(), &render_pass_create_info, nullptr, &render_pass_handle_) !=
  //  VK_SUCCESS)
  //  {
  //    LOG_ERROR("Failed to create Vulkan Render Pass");
  //    return false;
  //  }

  return true;
}

std::vector<VkSubpassDescription> VulkanRenderPass::GetSubPassDescriptions() const
{
  std::vector<VkSubpassDescription> sub_pass_handles;
  for (const auto& sub_pass_info : sub_pass_infos_)
  {
    const auto& sub_pass = sub_pass_info.sub_pass;
    sub_pass_handles.emplace_back(sub_pass->GetDescription());
  }

  return sub_pass_handles;
}

VkRenderPass VulkanRenderPass::GetHandle()
{
  if (!render_pass_handle_)
  {
    CreateRenderPass();
  }
  return render_pass_handle_;
}

void VulkanRenderPass::CreateRenderPass()
{
  CreateFrameBuffer();
}

void VulkanRenderPass::CreateFrameBuffer()
{
  if (has_multiple_attachments_)
  {
    frame_buffer_group_ =
      std::make_unique<FrameBufferGroup>(device_, shared_from_this(), extent_, swapchain_->GetImageCount());
  }
  else
  {
    frame_buffer_group_ = std::make_unique<FrameBufferGroup>(device_, shared_from_this(), extent_, 1);
  }

//  for (const auto& image_info : attachment_infos_)
//  {
//    if (image_info.IsMultipleAttachment())
//    {
//      const auto& images = std::get<1>(image_info.attachment); // TODO replace by method
//      const auto& image = images[0]; // Assuming at least one image
//
//      const auto type = image->GetAttachmentType();
//
//      if ((type & AttachmentType::Color) != AttachmentType::None) // bruh why
//      {
//        //frame_buffer_group_->AddColorImages(images);
//      }
//      else
//      {
//        LOG_ERROR("Unsupported attachment type for multiple images");
//      }
//    }
//    else
//    {
//      const auto& image = std::get<0>(image_info.attachment);
//      const auto type = image->GetAttachmentType();
//
//      if ((type & AttachmentType::Color) != AttachmentType::None)
//      {
//        //frame_buffer_group_->AddSharedColorImage( image );
//      }
//      else if ((type & AttachmentType::Depth) != AttachmentType::None)
//      {
//        //frame_buffer_group_->SetSharedDepthImage(image);
//      }
//      else
//      {
//        LOG_ERROR("Unsupported attachment type for image");
//      }
//    }
//  }
}

AttachmentHandle VulkanRenderPass::AddImage(std::shared_ptr<VulkanImage> image)
{
  if (CheckNotInitialized()) return {};

  const auto handle = AttachmentHandle { next_attachment_handle_++ };
//  attachment_infos_.emplace_back(handle, image);
  return handle;
}

AttachmentHandle VulkanRenderPass::AddSwapchainImages(
  uint32_t attachment_index, std::shared_ptr<VulkanSwapchain> swapchain)
{
  if (CheckNotInitialized()) return {};
  if (swapchain->GetImageCount() == 0)
  {
    LOG_ERROR("Cannot add swapchain images to render pass, swapchain has no images");
    return {};
  }

  has_multiple_attachments_ = swapchain->GetImageCount() > 1;
  swapchain_ = std::move(swapchain);

  const auto handle = AttachmentHandle { next_attachment_handle_++ };
  //attachment_infos_.emplace_back(handle, swapchain_->GetAttachments(attachment_index)); deprecated
  return handle;
}

SubPassHandle VulkanRenderPass::AddSubPass()
{
  if (CheckNotInitialized()) return {};

  const auto handle = SubPassHandle { next_subpass_handle_++ };
  //sub_pass_infos_.emplace_back(handle, std::make_shared<VulkanSubPass>());
  return handle;
}

// TODO return a subpass proxy instead of a subpass handle to avoid having to pass the subpass handle ???
// TODO check if handles are valid
void VulkanRenderPass::AddSubPassInput(SubPassHandle sub_pass_handle, AttachmentHandle image_handle)
{
  if (CheckNotInitialized()) return;
  return;

  const auto& sub_pass_info = FindSubPassInfo(sub_pass_handle);
  if (!sub_pass_info.valid) return;

  const auto& image_info = FindImageInfo(image_handle);
  //if (!image_info.valid) return;

  access_infos_.emplace_back(sub_pass_handle, image_handle, AccessType::Read);
}

void VulkanRenderPass::AddSubPassOutput(SubPassHandle sub_pass_handle, AttachmentHandle image_handle)
{
  if (CheckNotInitialized()) return;

  const auto& sub_pass_info = FindSubPassInfo(sub_pass_handle);
  if (!sub_pass_info.valid) return;

  const auto& image_info = FindImageInfo(image_handle);
  //if (!image_info.valid) return;

  access_infos_.emplace_back(sub_pass_handle, image_handle, AccessType::Write);
}

void VulkanRenderPass::AddAttachmentToSubPass(
  const std::shared_ptr<VulkanSubPass>& sub_pass, std::shared_ptr<VulkanRenderingBuffer> attachment)
{
  //  const auto type = attachment->GetAttachmentType();
  //  using AttachmentType = VulkanAttachment::AttachmentType;
  //  switch (type)
  //  {
  //  case AttachmentType::Color:
  //    sub_pass->AddColorAttachment(std::move(std::dynamic_pointer_cast<VulkanColorAttachment>(std::move(attachment))));
  //    break;
  //  case AttachmentType::Depth:
  //    sub_pass->AddDepthAttachment(std::move(std::dynamic_pointer_cast<VulkanDepthAttachment>(std::move(attachment))));
  //    break;
  //  default: LOG_WARN("Unimplemented attachment type: " + std::to_string(static_cast<int>(type))); break;
  //  }
}

VulkanRenderPass::SubPassInfo VulkanRenderPass::FindSubPassInfo(SubPassHandle sub_pass_handle)
{
  for (auto& sub_pass_info : sub_pass_infos_)
  {
    if (sub_pass_info.handle.value == sub_pass_handle.value)
    {
      return sub_pass_info;
    }
  }

  LOG_ERROR("Failed to find sub pass with handle: " + std::to_string(sub_pass_handle.value));
  return {};
}

VulkanRenderPass::AttachmentInfo VulkanRenderPass::FindImageInfo(AttachmentHandle image_handle)
{
  for (auto& image_info : attachment_infos_)
  {
//    if (image_info.handle.value == image_handle.value)
//    {
//      return image_info;
//    }
  }

  LOG_ERROR("Failed to find sub pass image with handle: " + std::to_string(image_handle.value));
  return {};
}

// TODO better name
bool VulkanRenderPass::CheckNotInitialized()
{
  if (render_pass_handle_)
  {
    LOG_ERROR("Cannot modify render pass after calling GetHandle()");
  }
  return render_pass_handle_;
}

void VulkanRenderPass::AddAccess(const VulkanRenderPass::AccessInfo& access_info) {}

} // namespace genebits::engine