#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_RENDER_PASS_2_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_RENDER_PASS_2_H

#include "genebits/engine/graphics/vulkan/attachments/vulkan_attachment_2.h"
#include "genebits/engine/graphics/vulkan/framebuffer/vulkan_frame_buffer_2.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include <memory>
#include <utility>
#include <vector>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanRenderPass2
{
public:
  VulkanRenderPass2(std::shared_ptr<VulkanDevice> device, VkExtent2D extent, uint32_t frame_count)
    : frame_buffer_(std::make_shared<VulkanFrameBuffer2>(device, extent, frame_count)), device_(std::move(device))
  {}

  [[nodiscard]] VkRenderPass GetHandle() const noexcept
  {
    return render_pass_handle_;
  }

  [[nodiscard]] std::vector<VkSubpassDescription> GetSubPassDescriptions() const
  {
    return {};
  }

private:
  VkRenderPass render_pass_handle_;
  std::shared_ptr<VulkanFrameBuffer2> frame_buffer_;
  std::shared_ptr<VulkanDevice> device_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_RENDER_PASS_2_H
