#include "vulkan_device.h"

#include "plex/debug/logging.h"
#include "plex/graphics/vulkan/vulkan_config.h"
#include "plex/graphics/vulkan/vulkan_surface.h"
#include "plex/graphics/vulkan/vulkan_swapchain.h"

#include <array>
#include <memory>
#include <set>
#include <vector>
#include <optional>

namespace plex
{

VulkanDevice::VulkanDevice(std::shared_ptr<VulkanInstance> instance,
  std::shared_ptr<VulkanSurface> surface,
  SwapchainImageUsage swapchain_image_usage)
  : instance_(instance)
{
  const auto swapchain_image_usage_flag = ConvertSwapchainUsage(swapchain_image_usage);
  if (!PickPhysicalDevice(instance, surface, swapchain_image_usage_flag))
  {
    LOG_ERROR("Failed to find a suitable vulkan physical device");

    return;
  }

  if (!Initialize(surface))
  {
    LOG_ERROR("Failed to initialize vulkan logical device");

    return;
  }

  if (!InitializeVMA(instance))
  {
    LOG_ERROR("Failed to initialize vulkan memory allocator");

    return;
  }

  LOG_INFO("Vulkan device initialized");
}

VulkanDevice::~VulkanDevice()
{
  vmaDestroyAllocator(vma_allocator_);
  vma_allocator_ = nullptr;

  vkDestroyDevice(logical_device_, nullptr);
  logical_device_ = VK_NULL_HANDLE;

  LOG_INFO("Vulkan device destroyed");
}

bool VulkanDevice::PickPhysicalDevice(const std::shared_ptr<VulkanInstance>& instance,
  const std::shared_ptr<VulkanSurface>& surface,
  const VkImageUsageFlags swapchain_image_usage)
{
  std::vector<std::pair<VkPhysicalDevice, uint32_t>> candidates;

  for (const auto& physical_device : GetAvailablePhysicalDevices(instance->GetHandle()))
  {
    if (IsPhysicalDeviceSupported(physical_device, surface->GetHandle(), swapchain_image_usage))
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

  // Enable non-uniform indexing
  VkPhysicalDeviceDescriptorIndexingFeatures descriptor_indexing_features {};
  descriptor_indexing_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
  descriptor_indexing_features.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
  descriptor_indexing_features.runtimeDescriptorArray = VK_TRUE;
  descriptor_indexing_features.descriptorBindingVariableDescriptorCount = VK_TRUE;
  descriptor_indexing_features.descriptorBindingPartiallyBound = VK_TRUE;
  descriptor_indexing_features.descriptorBindingUpdateUnusedWhilePending = VK_TRUE;
  descriptor_indexing_features.descriptorBindingStorageImageUpdateAfterBind = VK_TRUE;

  create_info.pNext = &descriptor_indexing_features;

#ifndef NDEBUG
  create_info.enabledLayerCount = static_cast<uint32_t>(cVulkanValidationLayers.size());
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
  vkGetDeviceQueue(logical_device_, queue_family_indices_.compute_family_index, 0, &compute_queue_);

  const auto limits = GetPhysicalDeviceLimits();

  const auto& max_compute_work_group_count = limits.maxComputeWorkGroupCount;
  std::copy(max_compute_work_group_count, max_compute_work_group_count + 3, std::begin(max_compute_work_group_count_));

  const auto& max_compute_work_group_size = limits.maxComputeWorkGroupSize;
  std::copy(max_compute_work_group_size, max_compute_work_group_size + 3, std::begin(max_compute_work_group_size_));

  max_compute_work_group_invocations_ = limits.maxComputeWorkGroupInvocations;

  return true;
}

bool VulkanDevice::InitializeVMA(const std::shared_ptr<VulkanInstance>& instance)
{
  VmaVulkanFunctions vulkanFunctions = {};
  vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
  vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;
  vulkanFunctions.vkGetPhysicalDeviceProperties = &vkGetPhysicalDeviceProperties;
  vulkanFunctions.vkGetPhysicalDeviceMemoryProperties = &vkGetPhysicalDeviceMemoryProperties;
  vulkanFunctions.vkAllocateMemory = &vkAllocateMemory;
  vulkanFunctions.vkFreeMemory = &vkFreeMemory;
  vulkanFunctions.vkMapMemory = &vkMapMemory;
  vulkanFunctions.vkUnmapMemory = &vkUnmapMemory;
  vulkanFunctions.vkFlushMappedMemoryRanges = &vkFlushMappedMemoryRanges;
  vulkanFunctions.vkInvalidateMappedMemoryRanges = &vkInvalidateMappedMemoryRanges;
  vulkanFunctions.vkBindBufferMemory = &vkBindBufferMemory;
  vulkanFunctions.vkBindImageMemory = &vkBindImageMemory;
  vulkanFunctions.vkGetBufferMemoryRequirements = &vkGetBufferMemoryRequirements;
  vulkanFunctions.vkGetImageMemoryRequirements = &vkGetImageMemoryRequirements;
  vulkanFunctions.vkCreateBuffer = &vkCreateBuffer;
  vulkanFunctions.vkDestroyBuffer = &vkDestroyBuffer;
  vulkanFunctions.vkCreateImage = &vkCreateImage;
  vulkanFunctions.vkDestroyImage = &vkDestroyImage;
  vulkanFunctions.vkCmdCopyBuffer = &vkCmdCopyBuffer;
  vulkanFunctions.vkGetBufferMemoryRequirements2KHR =
    GetFunctionPointer<PFN_vkGetBufferMemoryRequirements2>("vkGetBufferMemoryRequirements2");
  vulkanFunctions.vkGetImageMemoryRequirements2KHR =
    GetFunctionPointer<PFN_vkGetImageMemoryRequirements2>("vkGetImageMemoryRequirements2");
  vulkanFunctions.vkBindBufferMemory2KHR = GetFunctionPointer<PFN_vkBindBufferMemory2>("vkBindBufferMemory2");
  vulkanFunctions.vkBindImageMemory2KHR = GetFunctionPointer<PFN_vkBindImageMemory2>("vkBindImageMemory2");
  vulkanFunctions.vkGetPhysicalDeviceMemoryProperties2KHR =
    GetFunctionPointer<PFN_vkGetPhysicalDeviceMemoryProperties2KHR>("vkGetPhysicalDeviceMemoryProperties2KHR");
  vulkanFunctions.vkGetDeviceBufferMemoryRequirements =
    GetFunctionPointer<PFN_vkGetDeviceBufferMemoryRequirements>("vkGetDeviceBufferMemoryRequirements");

  VmaAllocatorCreateInfo allocator_create_info = {};
  allocator_create_info.vulkanApiVersion = VK_API_VERSION_1_3;
  allocator_create_info.physicalDevice = physical_device_;
  allocator_create_info.device = logical_device_;
  allocator_create_info.instance = instance->GetHandle();
  allocator_create_info.pVulkanFunctions = &vulkanFunctions;
  allocator_create_info.flags = VmaAllocatorCreateFlagBits::VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;

  return vmaCreateAllocator(&allocator_create_info, &vma_allocator_) == VK_SUCCESS;
}

bool VulkanDevice::IsPhysicalDeviceSupported(
  VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkImageUsageFlags usage)
{
  if (!FindQueueFamilies(physical_device, surface, nullptr)) return false;

  for (const auto& required_extension : cVulkanDeviceExtensions)
  {
    if (!IsExtensionSupported(physical_device, required_extension)) return false;
  }

  VulkanSwapChainSupportDetails swap_chain_support = QuerySwapChainSupport(physical_device, surface);

  if (swap_chain_support.formats.empty() || swap_chain_support.present_modes.empty()) return false;

  if (!(swap_chain_support.capabilities.supportedUsageFlags & usage)) return false;

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
  std::optional<uint32_t> compute_family_index;

  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

  std::vector<VkQueueFamilyProperties> queue_families_properties(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families_properties.data());

  for (uint32_t i = 0; i < queue_family_count; ++i)
  {
    if (queue_families_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) graphics_family_index = i;
    if (queue_families_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) compute_family_index = i;

    VkBool32 is_present_surface_supported = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &is_present_surface_supported);

    if (is_present_surface_supported) present_family_index = i;

    if (graphics_family_index.has_value() && present_family_index.has_value() && compute_family_index.has_value())
    {
      if (indices)
        *indices = {
          graphics_family_index.value(),
          present_family_index.value(),
          compute_family_index.value(),
        };

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

VkPhysicalDeviceLimits VulkanDevice::GetPhysicalDeviceLimits() const
{
  VkPhysicalDeviceProperties physical_device_properties_;
  vkGetPhysicalDeviceProperties(physical_device_, &physical_device_properties_);
  return physical_device_properties_.limits;
}

std::vector<VkPhysicalDevice> VulkanDevice::GetAvailablePhysicalDevices(VkInstance instance)
{
  uint32_t physical_device_count = 0;
  vkEnumeratePhysicalDevices(instance, &physical_device_count, nullptr);

  std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
  vkEnumeratePhysicalDevices(instance, &physical_device_count, physical_devices.data());

  return physical_devices;
}
} // namespace plex
