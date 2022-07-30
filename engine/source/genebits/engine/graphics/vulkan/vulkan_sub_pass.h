#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_SUB_PASS_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_SUB_PASS_H

#include "genebits/engine/graphics/vulkan/attachments/vulkan_attachment_type.h"
#include "genebits/engine/graphics/vulkan/images/vulkan_color_attachment.h"
#include "genebits/engine/graphics/vulkan/images/vulkan_depth_attachment.h"
#include "genebits/engine/graphics/vulkan/images/vulkan_present_attachment.h"
#include "genebits/engine/graphics/vulkan/resources/vulkan_resource.h"
#include "genebits/engine/graphics/vulkan/shaders/vulkan_shader_program.h"

#include "genebits/engine/debug/assertion.h"
#include "genebits/engine/debug/logging.h"

#include "vulkan/vulkan_core.h"

#include <memory>
#include <utility>
#include <vector>

namespace genebits::engine
{

class VulkanSubPass
{
  friend class VulkanRenderPass2;
  friend class VulkanRenderPass; // TODO: Remove this dependency

public:
  VulkanSubPass(std::shared_ptr<VulkanShaderProgram> shader_program) : shader_program_(std::move(shader_program))
  {
    subpass_description_.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass_description_.colorAttachmentCount = 0;
    subpass_description_.pColorAttachments = color_attachment_references_.data();
  }

  void AddColorAttachment(std::shared_ptr<VulkanRenderingBuffer> color_attachment)
  {
    ASSERT((color_attachment->GetAttachmentType() & AttachmentType::Color) != AttachmentType::None,
      "Color attachment must be of type color");
    // TODO this is redundant, use
    //  color_attachment->GetAttachmentReference() when it's implemented
    color_attachment_references_.emplace_back(color_attachment->GetAttachmentReference());
    color_attachments_.emplace_back(std::move(color_attachment));
    subpass_description_.colorAttachmentCount = color_attachments_.size();
  }

  void AddDepthAttachment(std::shared_ptr<VulkanRenderingBuffer> depth_attachment)
  {
    ASSERT((depth_attachment->GetAttachmentType() & AttachmentType::Depth) != AttachmentType::None,
      "Color attachment must be of type color");
    if (depth_attachment_)
      LOG_WARN("Depth attachment already set for sub pass, previous depth attachment will be overwritten");

    depth_attachment_ = std::move(depth_attachment);
    subpass_description_.pDepthStencilAttachment = &depth_attachment_->GetAttachmentReference();
  }

private:
  [[nodiscard]] const VkSubpassDescription& GetDescription() const noexcept
  {
    return subpass_description_;
  }

protected:
  std::shared_ptr<VulkanShaderProgram> shader_program_;

  std::vector<VkAttachmentReference> color_attachment_references_;

  std::vector<std::shared_ptr<VulkanRenderingBuffer>> color_attachments_;
  std::shared_ptr<VulkanRenderingBuffer> depth_attachment_;

  std::shared_ptr<VulkanResource> input_resource_;
  std::shared_ptr<VulkanResource> output_resource_;

  VkSubpassDescription subpass_description_ {};
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_SUB_PASS_H
