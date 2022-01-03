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
    VkInstance instance_handle, VkSurfaceKHR vulkan_surface_handle, const std::vector<std::string>& required_extensions)
  {
    struct ScoredPhysicalDevice
    {
      uint32_t score;
      VkPhysicalDevice physical_device_handle;
      ScoredPhysicalDevice(uint32_t score, VkPhysicalDevice adapter_handle)
        : score(score), physical_device_handle(adapter_handle) {};
    };

    std::vector<VkPhysicalDevice> available_physical_device_handles = GetPhysicalDevices(instance_handle);

    if (available_physical_device_handles.empty()) { LOG_ERROR("Failed to find GPUs with Vulkan support"); }

    std::vector<ScoredPhysicalDevice> physical_device_candidates;

    for (const auto& physical_device_handle : available_physical_device_handles)
    {
      if (IsDeviceCandidate(physical_device_handle, vulkan_surface_handle, required_extensions))
      {
        uint32_t physical_device_suitability_score = GetPhysicalDeviceSuitabilityScore(physical_device_handle);
        physical_device_candidates.emplace_back(physical_device_suitability_score, physical_device_handle);
      }
    }

    if (physical_device_candidates.empty()) { LOG_ERROR("Failed to find a suitable GPU"); }

    std::ranges::sort(physical_device_candidates,
      [](ScoredPhysicalDevice left, ScoredPhysicalDevice right) { return std::cmp_less(left.score, right.score); });

    ScoredPhysicalDevice most_suitable_physical_device = physical_device_candidates.front();

    if (!most_suitable_physical_device.score) { LOG_ERROR("Failed to find a suitable GPU"); }

    return most_suitable_physical_device.physical_device_handle;
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

    for (std::size_t i = 0; i < queue_family_count; ++i)
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

private:
  static std::vector<VkPhysicalDevice> GetPhysicalDevices(VkInstance instance_handle)
  {
    uint32_t physical_device_count = 0;
    vkEnumeratePhysicalDevices(instance_handle, &physical_device_count, nullptr);

    std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
    vkEnumeratePhysicalDevices(instance_handle, &physical_device_count, physical_devices.data());

    return physical_devices;
  }

  static bool IsDeviceCandidate(
    VkPhysicalDevice adapter_handle, VkSurfaceKHR surface_handle, const std::vector<std::string>& required_extensions)
  {
    if (!GetAdapterQueueFamilyIndices(adapter_handle, surface_handle).IsGraphicsAndPresentFamilyIndexPresent())
    {
      return false;
    }

    if (!CheckPhysicalDeviceExtensionSupport(adapter_handle, required_extensions)) { return false; }

    VulkanSwapChainSupportDetails swap_chain_support_details =
      GetAdapterSwapChainSupportDetails(adapter_handle, surface_handle);

    if (swap_chain_support_details.formats.empty() || swap_chain_support_details.present_modes.empty())
    {
      return false;
    }

    VkPhysicalDeviceFeatures physical_device_supported_features;
    vkGetPhysicalDeviceFeatures(adapter_handle, &physical_device_supported_features);

    if (!physical_device_supported_features.samplerAnisotropy) { return false; }

    return true;
  }

  static bool CheckPhysicalDeviceExtensionSupport(
    VkPhysicalDevice adapter_handle, const std::vector<std::string>& required_extensions)
  {
    uint32_t available_extension_count = 0;

    vkEnumerateDeviceExtensionProperties(adapter_handle, nullptr, &available_extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(available_extension_count);
    vkEnumerateDeviceExtensionProperties(
      adapter_handle, nullptr, &available_extension_count, available_extensions.data());

    std::size_t found_extension_count = 0;
    for (const auto& extension : available_extensions)
    {
      if (std::ranges::find(required_extensions, extension.extensionName) != required_extensions.end())
      {
        ++found_extension_count;

        if (found_extension_count == required_extensions.size()) { return true; }
      }
    }

    return false;
  }

  static uint32_t GetPhysicalDeviceSuitabilityScore(VkPhysicalDevice adapter_handle)
  {
    uint32_t score = 0;

    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceProperties(adapter_handle, &device_properties);
    vkGetPhysicalDeviceFeatures(adapter_handle, &device_features);

    // Bellow is the scoring of features and properties, add as needed

    // Discrete GPUs have a significant performance advantage
    if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) { score += 1000; }

    return score;
  }
};

} // namespace genebits::engine
#endif
