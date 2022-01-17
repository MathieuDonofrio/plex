#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_SUB_PASS_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_SUB_PASS_H

#include "genebits/engine/graphics/vulkan/vulkan_attachment.h"
#include "genebits/engine/graphics/vulkan/vulkan_color_attachment.h" // TMP
#include "genebits/engine/graphics/vulkan/vulkan_present_attachment.h" // TMP

#include <vector>

#include <vulkan/vulkan_core.h>

namespace genebits::engine {

class VulkanSubPass {
public:
protected:
  std::vector<VulkanAttachment*> attachments_;
};

}

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_SUB_PASS_H
