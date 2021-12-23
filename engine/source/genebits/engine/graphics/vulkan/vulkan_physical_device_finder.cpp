#include "vulkan_physical_device_finder.h"

#include <algorithm>
#include <optional>

#include "genebits/engine/debug/logging.h"

namespace genebits::engine
{

VkPhysicalDevice PhysicalDeviceFinder::FindDevice(
  VkInstance vulkan_handle, VkSurfaceKHR vulkan_surface_handle, const std::vector<std::string>& required_extensions)
{
  struct ScoredPhysicalDevice
  {
    uint32_t score;
    VkPhysicalDevice physical_device_handle;
    ScoredPhysicalDevice(uint32_t score, VkPhysicalDevice physical_device_handle)
      : score(score), physical_device_handle(physical_device_handle) {};
  };

  std::vector<VkPhysicalDevice> available_physical_device_handles = GetPhysicalDevices(vulkan_handle);

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

std::vector<VkPhysicalDevice> PhysicalDeviceFinder::GetPhysicalDevices(VkInstance vulkan_handle)
{
  uint32_t physical_device_count = 0;
  vkEnumeratePhysicalDevices(vulkan_handle, &physical_device_count, nullptr);

  std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
  vkEnumeratePhysicalDevices(vulkan_handle, &physical_device_count, physical_devices.data());

  return physical_devices;
}

bool PhysicalDeviceFinder::IsDeviceCandidate(VkPhysicalDevice vulkan_physical_device_handle,
  VkSurfaceKHR vulkan_surface_handle,
  const std::vector<std::string>& required_extensions)
{
  if (!FindPhysicalDeviceQueueFamilyIndices(vulkan_physical_device_handle, vulkan_surface_handle)
         .IsGraphicsAndPresentFamilyIndexPresent())
  {
    return false;
  }

  if (!CheckPhysicalDeviceExtensionSupport(vulkan_physical_device_handle, required_extensions)) { return false; }

  VulkanSwapChainSupportDetails swap_chain_support_details =
    GetPhysicalDeviceSwapChainSupportDetails(vulkan_physical_device_handle, vulkan_surface_handle);

  if (swap_chain_support_details.formats.empty() || swap_chain_support_details.present_modes.empty()) { return false; }

  VkPhysicalDeviceFeatures physical_device_supported_features;
  vkGetPhysicalDeviceFeatures(vulkan_physical_device_handle, &physical_device_supported_features);

  if (!physical_device_supported_features.samplerAnisotropy) { return false; }

  return true;
}

VulkanQueueFamilyIndices PhysicalDeviceFinder::FindPhysicalDeviceQueueFamilyIndices(
  VkPhysicalDevice vulkan_physical_device_handle, VkSurfaceKHR vulkan_surface_handle)
{
  std::optional<uint32_t> graphics_family_index;
  std::optional<uint32_t> present_family_index;

  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(vulkan_physical_device_handle, &queue_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families_properties(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(
    vulkan_physical_device_handle, &queue_family_count, queue_families_properties.data());

  for (std::size_t i = 0; i < queue_family_count; ++i)
  {
    if (queue_families_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) { graphics_family_index = i; }

    VkBool32 is_present_surface_supported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(
      vulkan_physical_device_handle, i, vulkan_surface_handle, &is_present_surface_supported);

    if (is_present_surface_supported) { present_family_index = i; }

    if (graphics_family_index.has_value() && present_family_index.has_value())
    {
      return { graphics_family_index, present_family_index };
    }
  }

  return {};
}

bool PhysicalDeviceFinder::CheckPhysicalDeviceExtensionSupport(
  VkPhysicalDevice vulkan_physical_device_handle, const std::vector<std::string>& required_extensions)
{
  uint32_t available_extension_count = 0;

  vkEnumerateDeviceExtensionProperties(vulkan_physical_device_handle, nullptr, &available_extension_count, nullptr);

  std::vector<VkExtensionProperties> available_extensions(available_extension_count);
  vkEnumerateDeviceExtensionProperties(
    vulkan_physical_device_handle, nullptr, &available_extension_count, available_extensions.data());

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

VulkanSwapChainSupportDetails PhysicalDeviceFinder::GetPhysicalDeviceSwapChainSupportDetails(
  VkPhysicalDevice vulkan_physical_device_handle, VkSurfaceKHR vulkan_surface_handle)
{
  VulkanSwapChainSupportDetails swap_chain_support_details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
    vulkan_physical_device_handle, vulkan_surface_handle, &swap_chain_support_details.capabilities);

  uint32_t available_surface_format_count;
  vkGetPhysicalDeviceSurfaceFormatsKHR(
    vulkan_physical_device_handle, vulkan_surface_handle, &available_surface_format_count, nullptr);

  if (available_surface_format_count)
  {
    swap_chain_support_details.formats.resize(available_surface_format_count);

    vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan_physical_device_handle,
      vulkan_surface_handle,
      &available_surface_format_count,
      swap_chain_support_details.formats.data());
  }

  uint32_t available_surface_present_mode_count;
  vkGetPhysicalDeviceSurfacePresentModesKHR(
    vulkan_physical_device_handle, vulkan_surface_handle, &available_surface_present_mode_count, nullptr);

  if (available_surface_present_mode_count)
  {
    swap_chain_support_details.present_modes.resize(available_surface_present_mode_count);

    vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan_physical_device_handle,
      vulkan_surface_handle,
      &available_surface_present_mode_count,
      swap_chain_support_details.present_modes.data());
  }

  return swap_chain_support_details;
}

uint32_t PhysicalDeviceFinder::GetPhysicalDeviceSuitabilityScore(VkPhysicalDevice vulkan_physical_device_handle)
{
  uint32_t score = 0;

  VkPhysicalDeviceProperties device_properties;
  VkPhysicalDeviceFeatures device_features;
  vkGetPhysicalDeviceProperties(vulkan_physical_device_handle, &device_properties);
  vkGetPhysicalDeviceFeatures(vulkan_physical_device_handle, &device_features);

  // Bellow is the scoring of features and properties, add as needed

  // Discrete GPUs have a significant performance advantage
  if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) { score += 1000; }

  return score;
}

} // namespace genebits::engine