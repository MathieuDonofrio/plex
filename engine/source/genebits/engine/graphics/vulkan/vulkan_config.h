#ifndef GENEBITS_VULKAN_CONFIG_H
#define GENEBITS_VULKAN_CONFIG_H

#include "genebits/engine/config/compiler.h"

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

const std::vector<const char*> cVulkanValidationLayers = {
  "VK_LAYER_KHRONOS_validation",
};

const std::vector<const char*> cVulkanDeviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
  VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
};

} // namespace genebits::engine

#endif // GENEBITS_VULKAN_CONFIG_H
