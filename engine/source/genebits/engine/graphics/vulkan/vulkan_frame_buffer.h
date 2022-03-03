#ifndef GENEBITS_ENGINE_GRPAHICS_VULKANFRAMEBUFFER_H
#define GENEBITS_ENGINE_GRPAHICS_VULKANFRAMEBUFFER_H

#include <memory>

#include "vulkan/vulkan_core.h"

namespace genebits::engine {

class VulkanFrameBuffer {
public:


private:
  std::shared_ptr<VkFramebuffer> frame_buffer_handle_;

};

}

#endif // GENEBITS_ENGINE_GRPAHICS_VULKANFRAMEBUFFER_H
