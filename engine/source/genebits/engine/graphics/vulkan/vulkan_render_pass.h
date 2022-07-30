#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_RENDER_PASS_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_RENDER_PASS_H

#include "genebits/engine/debug/logging.h"
#include "genebits/engine/graphics/vulkan/vulkan_sub_pass.h"
#include "genebits/engine/graphics/vulkan/vulkan_swapchain.h"
#include "vulkan_frame_buffer_group.h"

#include <memory>
#include <variant>
#include <vector>

namespace genebits::engine
{

class VulkanRenderPass : public std::enable_shared_from_this<VulkanRenderPass>
{
public:
  struct AttachmentHandle
  {
    const uint64_t value;
  };

  struct SubPassHandle
  {
    const uint64_t value;
  };

  explicit VulkanRenderPass(std::shared_ptr<VulkanDevice> device);

  VulkanRenderPass(const VulkanRenderPass&) = delete;
  VulkanRenderPass& operator=(const VulkanRenderPass&) = delete;
  VulkanRenderPass(VulkanRenderPass&&) = delete;
  VulkanRenderPass& operator=(VulkanRenderPass&&) = delete;

  virtual ~VulkanRenderPass() = default;

  [[nodiscard]] VkRenderPass GetHandle();

  [[nodiscard]] std::vector<VkSubpassDescription> GetSubPassDescriptions() const;

  AttachmentHandle AddImage(std::shared_ptr<VulkanImage> image);

  AttachmentHandle AddSwapchainImages(uint32_t attachment_index, std::shared_ptr<VulkanSwapchain> swapchain);

  SubPassHandle AddSubPass();

  void AddSubPassInput(SubPassHandle sub_pass_handle, AttachmentHandle image_handle);

  void AddSubPassOutput(SubPassHandle sub_pass_handle, AttachmentHandle image_handle);

private:
  bool Initialize();
  void CreateRenderPass();
  void CreateFrameBuffer();

  bool CheckNotInitialized();

  struct SubPassInfo
  {
    const SubPassHandle handle {};
    std::shared_ptr<VulkanSubPass> sub_pass;
    const bool valid { true };

    SubPassInfo(SubPassHandle handle, std::shared_ptr<VulkanSubPass> sub_pass)
      : handle(handle), sub_pass(std::move(sub_pass))
    {}

    SubPassInfo() : valid(false) {}
  };

  SubPassInfo FindSubPassInfo(SubPassHandle sub_pass_handle);

  using MultipleAttachment = std::vector<std::shared_ptr<VulkanAttachment2>>;
  using SingleAttachment = std::shared_ptr<VulkanAttachment2>;
  struct AttachmentInfo
  {
//    const AttachmentHandle handle {};
//    std::variant<SingleAttachment, MultipleAttachment> attachment;
//    const bool valid { true };
//
//    AttachmentInfo(const AttachmentHandle& handle, std::variant<SingleAttachment, MultipleAttachment> attachment)
//      : handle(handle), attachment(std::move(attachment))
//    {}
//
//    AttachmentInfo() : valid(false) {}
//
//    [[nodiscard]] bool IsMultipleAttachment() const
//    {
//      return attachment.index() == 1;
//    }
  };
  AttachmentInfo FindImageInfo(AttachmentHandle image_handle);

  enum class AccessType
  {
    Read,
    Write,
    Undefined
  };

  struct AccessInfo
  {
    const SubPassHandle sub_pass_handle;
    const AttachmentHandle image_handle;
    const AccessType access_type;
  };

  void AddAccess(const AccessInfo& access_info);

  static void AddAttachmentToSubPass(
    const std::shared_ptr<VulkanSubPass>& sub_pass, std::shared_ptr<VulkanRenderingBuffer> attachment);

private:
  VkRenderPass render_pass_handle_ { VK_NULL_HANDLE };
  std::shared_ptr<VulkanDevice> device_;
  std::shared_ptr<VulkanSwapchain> swapchain_;

  VkExtent2D extent_;

  std::shared_ptr<FrameBufferGroup> frame_buffer_group_;

  bool has_multiple_attachments_ { false };

  static uint64_t next_attachment_handle_;
  std::vector<AttachmentInfo> attachment_infos_;

  static uint64_t next_subpass_handle_;
  std::vector<SubPassInfo> sub_pass_infos_;

  std::vector<AccessInfo> access_infos_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_RENDER_PASS_H
