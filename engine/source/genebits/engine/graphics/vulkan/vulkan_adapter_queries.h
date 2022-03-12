#ifndef GENEBITS_ENGINE_GRAPHICS_PHYSICAL_DEVICE_FINDER_H
#define GENEBITS_ENGINE_GRAPHICS_PHYSICAL_DEVICE_FINDER_H

#include "genebits/engine/debug/logging.h"
#include "genebits/engine/graphics/vulkan/vulkan_queue_family_indices.h"
#include "genebits/engine/graphics/vulkan/vulkan_swap_chain_support_details.h"

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{
class VulkanAdapterQueries
{
public:
  static VkPhysicalDevice FindSuitableAdapter(
    VkInstance instance, VkSurfaceKHR surface, const std::vector<std::string>& required_extensions)
  {
    std::vector<std::pair<VkPhysicalDevice, uint32_t>> candidates;

    for (const auto& physical_device : GetPhysicalDevices(instance))
    {
      if (IsPhysicalDeviceSupported(physical_device, surface, required_extensions))
      {
        candidates.emplace_back(physical_device, ComputePhysicalDeviceScore(physical_device));
      }
    }

    if (candidates.empty())
    {
      LOG_ERROR("Failed to find a suitable GPU");

      return nullptr;
    }

    std::ranges::sort(candidates, [](auto lhs, auto rhs) { return std::cmp_greater(lhs.second, rhs.second); });

    return candidates.front().first;
  }

  static VulkanQueueFamilyIndices GetAdapterQueueFamilyIndices(
    VkPhysicalDevice adapter_handle, VkSurfaceKHR surface_handle)
  {
    std::optional<uint32_t> graphics_family_index;
    std::optional<uint32_t> present_family_index;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(adapter_handle, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families_properties(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(adapter_handle, &queue_family_count, queue_families_properties.data());

    for (uint32_t i = 0; i < queue_family_count; ++i)
    {
      if (queue_families_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) { graphics_family_index = i; }

      VkBool32 is_present_surface_supported = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(adapter_handle, i, surface_handle, &is_present_surface_supported);

      if (is_present_surface_supported) { present_family_index = i; }

      if (graphics_family_index.has_value() && present_family_index.has_value())
      {
        return { graphics_family_index, present_family_index };
      }
    }

    return {};
  }

  static VulkanSwapChainSupportDetails GetAdapterSwapChainSupportDetails(
    VkPhysicalDevice adapter_handle, VkSurfaceKHR surface_handle)
  {
    VulkanSwapChainSupportDetails swap_chain_support_details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(adapter_handle, surface_handle, &swap_chain_support_details.capabilities);

    uint32_t available_surface_format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(adapter_handle, surface_handle, &available_surface_format_count, nullptr);

    if (available_surface_format_count)
    {
      swap_chain_support_details.formats.resize(available_surface_format_count);

      vkGetPhysicalDeviceSurfaceFormatsKHR(
        adapter_handle, surface_handle, &available_surface_format_count, swap_chain_support_details.formats.data());
    }

    uint32_t available_surface_present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(
      adapter_handle, surface_handle, &available_surface_present_mode_count, nullptr);

    if (available_surface_present_mode_count)
    {
      swap_chain_support_details.present_modes.resize(available_surface_present_mode_count);

      vkGetPhysicalDeviceSurfacePresentModesKHR(adapter_handle,
        surface_handle,
        &available_surface_present_mode_count,
        swap_chain_support_details.present_modes.data());
    }

    return swap_chain_support_details;
  }

  static VkFormat FindSupportedFormat(VkPhysicalDevice adapter_handle,
    const std::vector<VkFormat>& format_candidates,
    VkImageTiling tiling,
    VkFormatFeatureFlags features)
  {
    for (VkFormat format : format_candidates)
    {
      VkFormatProperties format_properties;
      vkGetPhysicalDeviceFormatProperties(adapter_handle, format, &format_properties);

      if (tiling == VK_IMAGE_TILING_LINEAR && (format_properties.linearTilingFeatures & features) == features
          || tiling == VK_IMAGE_TILING_OPTIMAL && (format_properties.optimalTilingFeatures & features) == features)
      {
        return format;
      }
    }

    LOG_ERROR("Failed to find supported format");
  }

private:
  static bool IsPhysicalDeviceSupported(
    VkPhysicalDevice physical_device, VkSurfaceKHR surface, const std::vector<std::string>& required_extensions)
  {
    if (!GetAdapterQueueFamilyIndices(physical_device, surface).IsGraphicsAndPresentFamilyIndexPresent())
    {
      return false;
    }

    for (const auto& required_extension : required_extensions)
    {
      if (!IsExtensionSupported(physical_device, required_extension)) return false;
    }

    VulkanSwapChainSupportDetails swap_chain_support_details =
      GetAdapterSwapChainSupportDetails(physical_device, surface);

    if (swap_chain_support_details.formats.empty() || swap_chain_support_details.present_modes.empty())
    {
      return false;
    }

    VkPhysicalDeviceFeatures physical_device_supported_features;
    vkGetPhysicalDeviceFeatures(physical_device, &physical_device_supported_features);

    if (!physical_device_supported_features.samplerAnisotropy) return false;

    return true;
  }

  static bool IsExtensionSupported(VkPhysicalDevice physical_device, const std::string& extension_name)
  {
    uint32_t available_extension_count = 0;

    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &available_extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(available_extension_count);

    vkEnumerateDeviceExtensionProperties(
      physical_device, nullptr, &available_extension_count, available_extensions.data());

    for (const auto& extension : available_extensions)
    {
      if (extension_name == extension.extensionName) return true;
    }

    return false;
  }

  static uint32_t ComputePhysicalDeviceScore(VkPhysicalDevice physical_device)
  {
    uint32_t score = 0;

    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures device_features;

    vkGetPhysicalDeviceProperties(physical_device, &device_properties);
    vkGetPhysicalDeviceFeatures(physical_device, &device_features);

    // Bellow is the scoring of features and properties, add as needed

    score += device_properties.limits.maxImageDimension2D / 32;

    // Discrete GPUs have a significant performance advantage

    if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
      score *= 2;
      score += 1000;
    }

    return score;
  }

  static std::vector<VkPhysicalDevice> GetPhysicalDevices(VkInstance instance)
  {
    uint32_t physical_device_count = 0;
    vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);

    std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
    vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());

    return physical_devices;
  }
};

} // namespace genebits::engine
#endif
