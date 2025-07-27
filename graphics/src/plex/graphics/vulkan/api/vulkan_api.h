#ifndef PLEX_GRAPHICS_VULKAN_API_VULKAN_API_H
#define PLEX_GRAPHICS_VULKAN_API_VULKAN_API_H

#include <vector>

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>

namespace plex::graphics::vapi
{
const std::vector<const char*> cVulkanValidationLayers = {
  "VK_LAYER_KHRONOS_validation",
};

const std::vector<const char*> cVulkanDeviceExtensions = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
  VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
};

const std::vector<VkValidationFeatureEnableEXT> cVulkanValidationFeatures = {
  VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT,
};

// TODO vulkan functions
} // namespace plex::graphics::vapi

#endif
