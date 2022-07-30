#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_SUB_PASS_2_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_SUB_PASS_2_H

#include "genebits/engine/debug/logging.h"
#include "genebits/engine/graphics/vulkan/renderpass/vulkan_render_pass_2.h"

#include <memory>
#include <utility>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanSubPass2
{
  friend class VulkanRenderPass2;

public:
  VulkanSubPass2(const VulkanSubPass2&) = delete;
  VulkanSubPass2& operator=(const VulkanSubPass2&) = delete;
  VulkanSubPass2(VulkanSubPass2&&) = default;
  VulkanSubPass2& operator=(VulkanSubPass2&&) = default;

  ~VulkanSubPass2()
  {
    if (render_pass_handle_ != VK_NULL_HANDLE)
    {
      vkDestroyRenderPass(device_->GetHandle(), render_pass_handle_, nullptr);
    }
  }

  void WritesToColorAttachmentGroup(const VulkanAttachmentGroup& attachment_group)
  {
    frame_buffer_->AddColorAttachmentGroup(attachment_group);
  }

  void WritesToDepthAttachment(const VulkanAttachmentGroup& attachment_group)
  {
    frame_buffer_->AddDepthAttachmentGroup(attachment_group);
  }

  void ReadsFromColorAttachment(std::shared_ptr<VulkanAttachment2> color_attachment) {}

  [[nodiscard]] VkRenderPass GetHandle() const noexcept
  {
    return render_pass_handle_;
  }

  [[nodiscard]] std::vector<VkSubpassDescription> GetSubPassDescriptions() const
  {
    return {};
  }

private:
  VulkanSubPass2(std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanFrameBuffer2> frame_buffer)
    : device_(std::move(device)), frame_buffer_(std::move(frame_buffer))
  {}

private:
  VkRenderPass render_pass_handle_;
  std::shared_ptr<VulkanDevice> device_;
  std::shared_ptr<VulkanFrameBuffer2> frame_buffer_;

  enum class DependencyType
  {
    Read,
    Write,
  };

  struct Dependency
  {
    DependencyType type;
    VulkanAttachmentGroup group;
  };
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_SUB_PASS_2_H
