#ifndef GENEBITS_ENGINE_GRAPHICS_PHYSICAL_DEVICE_FINDER_H
#define GENEBITS_ENGINE_GRAPHICS_PHYSICAL_DEVICE_FINDER_H

#include <string>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "genebits/engine/graphics/vulkan_queue_family_indices.h"
#include "genebits/engine/graphics/vulkan_swap_chain_support_details.h"

namespace genebits::engine
{

class PhysicalDeviceFinder
{
public:
  static VkPhysicalDevice FindDevice(
    VkInstance vulkan_handle, VkSurfaceKHR vulkan_surface_handle, const std::vector<std::string>& required_extensions);

private:
  static std::vector<VkPhysicalDevice> GetPhysicalDevices(VkInstance vulkan_handle);

  static bool IsDeviceCandidate(VkPhysicalDevice vulkan_physical_device_handle,
    VkSurfaceKHR vulkan_surface_handle,
    const std::vector<std::string>& required_extensions);

  static VulkanQueueFamilyIndices FindPhysicalDeviceQueueFamilyIndices(
    VkPhysicalDevice vulkan_physical_device_handle, VkSurfaceKHR vulkan_surface_handle);

  static bool CheckPhysicalDeviceExtensionSupport(
    VkPhysicalDevice vulkan_physical_device_handle, const std::vector<std::string>& required_extensions);

  static VulkanSwapChainSupportDetails GetPhysicalDeviceSwapChainSupportDetails(
    VkPhysicalDevice vulkan_physical_device_handle, VkSurfaceKHR vulkan_surface_handle);

  static uint32_t GetPhysicalDeviceSuitabilityScore(VkPhysicalDevice vulkan_physical_device_handle);
};

} // namespace genebits::engine
#endif
