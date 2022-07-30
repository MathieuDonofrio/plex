#ifndef GENEBITS_RENDER_PASS_SEQUENCE_H
#define GENEBITS_RENDER_PASS_SEQUENCE_H

#include "genebits/engine/graphics/vulkan/vulkan_swapchain.h"

#include <memory>

namespace genebits::engine
{

class FrameBufferGroup;

// TODO better name that better represents the purpose of this class
class RenderPassSequence
{
public:
  RenderPassSequence(std::shared_ptr<VulkanDevice> device, std::shared_ptr<VulkanSwapchain> swapchain)
  {
//    auto render_pass = std::make_shared<VulkanRenderPass>(device, swapchain);
//    frame_buffer_group_ = std::make_unique<FrameBufferGroup>(device, swapchain, {});
  }

private:
  std::shared_ptr<FrameBufferGroup> frame_buffer_group_;
};

} // namespace genebits::engine
#endif // GENEBITS_RENDER_PASS_SEQUENCE_H
