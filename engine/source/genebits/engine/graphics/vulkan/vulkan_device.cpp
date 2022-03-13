#include "vulkan_device.h"

#include "genebits/engine/debug/logging.h"
#include "vulkan_config.h"
#include "vulkan_surface.h"
#include "vulkan_swapchain.h"

#include <array>
#include <memory>
#include <set>
#include <vector>

namespace genebits::engine
{
VulkanDevice::VulkanDevice(std::shared_ptr<VulkanInstance> instance, std::shared_ptr<VulkanSurface> surface)
{
  if (!PickPhysicalDevice(instance, surface))
  {
    LOG_ERROR("Failed to find a suitable vulkan physical device");

    return;
  }

  if (!Initialize(surface))
  {
    LOG_ERROR("Failed to initialize vulkan logical device");

    return;
  }

  LOG_INFO("Vulkan device initialized");
}

VulkanDevice::~VulkanDevice()
{
  vkDestroyDevice(logical_device_, nullptr);
  logical_device_ = VK_NULL_HANDLE;

  LOG_INFO("Vulkan device destroyed");
}

bool VulkanDevice::PickPhysicalDevice(
  const std::shared_ptr<VulkanInstance>& instance, const std::shared_ptr<VulkanSurface>& surface)
{
  std::vector<std::pair<VkPhysicalDevice, uint32_t>> candidates;

  for (const auto& physical_device : GetAvailablePhysicalDevices(instance->GetHandle()))
  {
    if (IsPhysicalDeviceSupported(physical_device, surface->GetHandle()))
    {
      candidates.emplace_back(physical_device, ComputePhysicalDeviceScore(physical_device));
    }
  }

  if (candidates.empty())
  {
    LOG_ERROR("Failed to find a suitable GPU");

    return false;
  }

  std::ranges::sort(candidates, [](auto lhs, auto rhs) { return std::cmp_greater(lhs.second, rhs.second); });

  physical_device_ = candidates.front().first;

  return true;
}

bool VulkanDevice::Initialize(const std::shared_ptr<VulkanSurface>& surface)
{
  if (!FindQueueFamilies(physical_device_, surface->GetHandle(), &queue_family_indices_))
  {
    LOG_ERROR("Failed to find queue families");

    return false;
  }

  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

  std::set<uint32_t> unique_queue_families = { queue_family_indices_.graphics_family_index,
    queue_family_indices_.present_family_index };

  const float queue_priority = 1.0f;

  for (uint32_t queue_family : unique_queue_families)
  {
    VkDeviceQueueCreateInfo queue_create_info {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(queue_create_info);
  }

  VkPhysicalDeviceFeatures device_features {};
  device_features.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo create_info {};
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
  create_info.pQueueCreateInfos = queue_create_infos.data();
  create_info.pEnabledFeatures = &device_features;
  create_info.enabledExtensionCount = static_cast<uint32_t>(cVulkanDeviceExtensions.size());
  create_info.ppEnabledExtensionNames = cVulkanDeviceExtensions.data();

#ifndef NDEBUG
  create_info.enabledLayerCount = cVulkanValidationLayers.size();
  create_info.ppEnabledLayerNames = cVulkanValidationLayers.data();
#else
  create_info.enabledLayerCount = 0;
#endif

  if (vkCreateDevice(physical_device_, &create_info, nullptr, &logical_device_) != VK_SUCCESS)
  {
    LOG_ERROR("Failed to create vulkan logical device");

    return false;
  }

  vkGetDeviceQueue(logical_device_, queue_family_indices_.graphics_family_index, 0, &graphics_queue_);
  vkGetDeviceQueue(logical_device_, queue_family_indices_.present_family_index, 0, &present_queue_);

  return true;
}

bool VulkanDevice::IsPhysicalDeviceSupported(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
  if (!FindQueueFamilies(physical_device, surface, nullptr)) return false;

  for (const auto& required_extension : cVulkanDeviceExtensions)
  {
    if (!IsExtensionSupported(physical_device, required_extension)) return false;
  }

  VulkanSwapChainSupportDetails swap_chain_support = QuerySwapChainSupport(physical_device, surface);

  if (swap_chain_support.formats.empty() || swap_chain_support.present_modes.empty()) return false;

  VkPhysicalDeviceFeatures physical_device_supported_features;
  vkGetPhysicalDeviceFeatures(physical_device, &physical_device_supported_features);

  if (!physical_device_supported_features.samplerAnisotropy) return false;

  return true;
}

bool VulkanDevice::FindQueueFamilies(
  VkPhysicalDevice physical_device, VkSurfaceKHR surface, VulkanQueueFamilyIndices* indices)
{
  std::optional<uint32_t> graphics_family_index;
  std::optional<uint32_t> present_family_index;

  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families_properties(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families_properties.data());

  for (uint32_t i = 0; i < queue_family_count; ++i)
  {
    if (queue_families_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) graphics_family_index = i;

    VkBool32 is_present_surface_supported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &is_present_surface_supported);

    if (is_present_surface_supported) present_family_index = i;

    if (graphics_family_index.has_value() && present_family_index.has_value())
    {
      if (indices) *indices = { graphics_family_index.value(), present_family_index.value() };

      return true;
    }
  }

  return false;
}

bool VulkanDevice::IsExtensionSupported(VkPhysicalDevice physical_device, const std::string& extension_name)
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

uint32_t VulkanDevice::ComputePhysicalDeviceScore(VkPhysicalDevice physical_device)
{
  uint32_t score = 0;

  VkPhysicalDeviceProperties device_properties;
  VkPhysicalDeviceFeatures device_features;

  vkGetPhysicalDeviceProperties(physical_device, &device_properties);
  vkGetPhysicalDeviceFeatures(physical_device, &device_features);

  // Discrete GPUs have a significant performance advantage

  if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score += 1000;

  // Bellow is the scoring of features and properties, add as needed

  score += device_properties.limits.maxImageDimension2D / 8;

  return score;
}

std::vector<VkPhysicalDevice> VulkanDevice::GetAvailablePhysicalDevices(VkInstance instance)
{
  uint32_t physical_device_count = 0;
  vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);

  std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
  vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());

  return physical_devices;
}
} // namespace genebits::engine
