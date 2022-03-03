#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_SUB_PASS_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_SUB_PASS_H

#include "genebits/engine/graphics/vulkan/vulkan_color_attachment.h" // TMP
#include "genebits/engine/graphics/vulkan/vulkan_depth_attachment.h"
#include "genebits/engine/graphics/vulkan/vulkan_present_attachment.h" // TMP

#include "genebits/engine/debug/logging.h"

#include <vector>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanSubPass
{
public:
  VulkanSubPass(std::vector<VulkanColorAttachment>&& color_attachments, VulkanDepthAttachment&& depth_attachment)
    : color_attachments_(std::move(color_attachments)), depth_attachment_(depth_attachment)
  {
    if (!Initialize()) { LOG_ERROR("Failed to initialize Vulkan surface"); }
    else
    {
      LOG_INFO("Vulkan surface initialized");
    }
  }

protected:
  bool Initialize()
  {
    std::vector<VkAttachmentReference> color_attachment_references_;
    color_attachment_references_.reserve(color_attachments_.size());
    for (const auto &color_attachment : color_attachments_) {
      color_attachment_references_.emplace_back(color_attachment.GetAttachmentReference());
    }

    VkSubpassDescription sub_pass {};
    sub_pass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    sub_pass.colorAttachmentCount = color_attachment_references_.size();
    sub_pass.pColorAttachments = color_attachment_references_.data();
    sub_pass.pDepthStencilAttachment = &depth_attachment_.GetAttachmentReference();
  }

  std::vector<VulkanColorAttachment> color_attachments_;
  VulkanDepthAttachment depth_attachment_;

  VkSubpassDescription subpass_description_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_SUB_PASS_H
