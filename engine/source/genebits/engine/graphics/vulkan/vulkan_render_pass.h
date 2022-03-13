#ifndef GENEBITS_ENGINE_GRAPHICS_VULKANRENDERPASS_H
#define GENEBITS_ENGINE_GRAPHICS_VULKANRENDERPASS_H

#include "genebits/engine/debug/logging.h"
#include "vulkan_attachment.h"
#include "vulkan_sub_pass.h"
#include "genebits/engine/graphics/vulkan/vulkan_frame_buffer.h"
#include "vulkan_frame_buffer.h"

namespace genebits::engine {

class VulkanRenderPass
{
public:

  virtual ~VulkanRenderPass() = default;


protected:
  std::shared_ptr<VulkanFrameBuffer> framebuffer_;
};

}


#endif // GENEBITS_ENGINE_GRAPHICS_VULKANRENDERPASS_H
