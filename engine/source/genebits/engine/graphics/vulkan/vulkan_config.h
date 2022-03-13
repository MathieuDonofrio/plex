//
// Created by Mathieu on 3/12/2022.
//

#ifndef GENEBITS_VULKAN_CONFIG_H
#define GENEBITS_VULKAN_CONFIG_H

#include <vulkan/vulkan_core.h>

#include "genebits/engine/config/compiler.h"

namespace genebits::engine
{
const std::vector<const char*> cVulkanValidationLayers = { "VK_LAYER_KHRONOS_validation" };

const std::vector<const char*> cVulkanDeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
} // namespace genebits::engine

#endif // GENEBITS_VULKAN_CONFIG_H
