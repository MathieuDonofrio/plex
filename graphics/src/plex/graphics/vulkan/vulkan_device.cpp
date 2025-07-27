#include "plex/graphics/vulkan/vulkan_device.h"

#include <algorithm>
#include <optional>
#include <set>
#include <vector>

#include "plex/debug/logging.h"
#include "plex/graphics/vulkan/vulkan_swapchain.h"

namespace plex::graphics
{
namespace
{
  struct QueueFamilyIndices
  {
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;
    std::optional<uint32_t> transfer_family;
    std::optional<uint32_t> compute_family;

    [[nodiscard]] bool IsComplete() const
    {
      return graphics_family.has_value() && present_family.has_value() && transfer_family.has_value()
             && compute_family.has_value();
    }
  };

  struct VulkanSwapChainSupportDetails
  {
    VkSurfaceCapabilitiesKHR capabilities;

    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
  };

  std::vector<VkPhysicalDevice> GetAvailablePhysicalDevices(VkInstance instance)
  {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    if (device_count == 0)
    {
      LOG_ERROR("No Vulkan compatible devices found");
      return {};
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    return devices;
  }

  uint32_t ComputePhysicalDeviceScore(VkPhysicalDevice physical_device)
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

  bool IsExtensionSupported(VkPhysicalDevice physical_device, const std::string& extension_name)
  {
    uint32_t extension_count = 0;
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, available_extensions.data());

    for (const auto& extension : available_extensions)
    {
      if (extension.extensionName == extension_name)
      {
        return true;
      }
    }

    return false;
  }

  void FindQueueFamilies(VkPhysicalDevice physical_device, VkSurfaceKHR surface, QueueFamilyIndices* indices)
  {
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

    for (int i = 0; i < queue_families.size(); ++i)
    {
      const auto& queue_family = queue_families[i];

      if (queue_family.queueCount > 0 && queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
      {
        indices->graphics_family = i;
      }

      VkBool32 present_support = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &present_support);

      if (queue_family.queueCount > 0 && present_support)
      {
        indices->present_family = i;
      }

      if (queue_family.queueCount > 0 && queue_family.queueFlags & VK_QUEUE_TRANSFER_BIT)
      {
        indices->transfer_family = i;
      }

      if (queue_family.queueCount > 0 && queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT)
      {
        indices->compute_family = i;
      }
    }
  }

  VulkanSwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
  {
    VulkanSwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.capabilities);

    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);

    if (format_count != 0)
    {
      details.formats.resize(format_count);

      vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, details.formats.data());
    }

    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);

    if (present_mode_count != 0)
    {
      details.present_modes.resize(present_mode_count);

      vkGetPhysicalDeviceSurfacePresentModesKHR(
        physical_device, surface, &present_mode_count, details.present_modes.data());
    }

    return details;
  }

  bool IsPhysicalDeviceSupported(VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkImageUsageFlags usage)
  {
    QueueFamilyIndices indices;
    FindQueueFamilies(physical_device, surface, &indices);

    if (!indices.IsComplete()) return false;

    for (const auto& required_extension : vapi::cVulkanDeviceExtensions)
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

  VkPhysicalDevice PickPhysicalDevice(
    VkInstance instance, VkSurfaceKHR surface, const VkImageUsageFlags swapchain_image_usage)
  {
    std::vector<std::pair<VkPhysicalDevice, uint32_t>> candidates;

    for (const auto& physical_device : GetAvailablePhysicalDevices(instance))
    {
      if (IsPhysicalDeviceSupported(physical_device, surface, swapchain_image_usage))
      {
        candidates.emplace_back(physical_device, ComputePhysicalDeviceScore(physical_device));
      }
    }

    if (candidates.empty())
    {
      LOG_ERROR("Failed to find a suitable GPU");

      return nullptr;
    }

    auto most_suitable = std::max_element(
      candidates.begin(), candidates.end(), [](const auto& a, const auto& b) { return a.second < b.second; });

    if (most_suitable == candidates.end())
    {
      LOG_ERROR("Failed to find a suitable GPU");

      return nullptr;
    }

    return most_suitable->first;
  }

  uint32_t FindMemoryType(VkPhysicalDevice device, uint32_t type_filter, VkMemoryPropertyFlags properties)
  {
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(device, &memory_properties);

    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)
    {
      if ((type_filter & (1 << i)) && (memory_properties.memoryTypes[i].propertyFlags & properties) == properties)
      {
        return i;
      }
    }

    LOG_ERROR("Failed to find suitable memory type");

    return 0;
  }

  constexpr VkBufferUsageFlagBits GetBufferUsageFlagBits(BufferUsageFlags usage) noexcept
  {
    int flags = 0;
    if ((usage & BufferUsageFlags::Vertex) != 0) flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    if ((usage & BufferUsageFlags::Index) != 0) flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    if ((usage & BufferUsageFlags::Uniform) != 0) flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    if ((usage & BufferUsageFlags::Storage) != 0) flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    if ((usage & BufferUsageFlags::TransferSource) != 0) flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    if ((usage & BufferUsageFlags::TransferDestination) != 0) flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    return static_cast<VkBufferUsageFlagBits>(flags);
  }

  constexpr VmaMemoryUsage GetVmaMemoryUsage(MemoryUsage memory_usage) noexcept
  {
    switch (memory_usage)
    {
    case MemoryUsage::Unknown: return VMA_MEMORY_USAGE_UNKNOWN;
    case MemoryUsage::GPU_Only: return VMA_MEMORY_USAGE_GPU_ONLY;
    case MemoryUsage::CPU_Only: return VMA_MEMORY_USAGE_CPU_ONLY;
    case MemoryUsage::CPU_To_GPU: return VMA_MEMORY_USAGE_CPU_TO_GPU;
    case MemoryUsage::GPU_To_CPU: return VMA_MEMORY_USAGE_GPU_TO_CPU;
    case MemoryUsage::CPU_Copy: return VMA_MEMORY_USAGE_CPU_COPY;
    case MemoryUsage::Auto: return VMA_MEMORY_USAGE_AUTO;
    }

    return VMA_MEMORY_USAGE_UNKNOWN;
  }
} // namespace

VulkanDevice::VulkanDevice(VkInstance instance, VkSurfaceKHR surface)
{
  // Pick physical device

  physical_device_ = PickPhysicalDevice(instance, surface, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);

  if (!physical_device_)
  {
    LOG_ERROR("Failed to find a suitable GPU");
    return;
  }

  // Create logical device

  QueueFamilyIndices queue_family_indices;
  FindQueueFamilies(physical_device_, surface, &queue_family_indices);

  graphics_queue_family_index_ = queue_family_indices.graphics_family.value();
  present_queue_family_index_ = queue_family_indices.present_family.value();
  transfer_queue_family_index_ = queue_family_indices.transfer_family.value();
  compute_queue_family_index_ = queue_family_indices.compute_family.value();

  std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
  std::set<uint32_t> unique_queue_families = {
    graphics_queue_family_index_, present_queue_family_index_, transfer_queue_family_index_, compute_queue_family_index_
  };

  float queue_priority = 1.0f;
  for (uint32_t queue_family : unique_queue_families)
  {
    VkDeviceQueueCreateInfo queue_create_info = {};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;
    queue_create_infos.push_back(queue_create_info);
  }

  VkPhysicalDeviceFeatures device_features = {};
  device_features.samplerAnisotropy = VK_TRUE;

  VkDeviceCreateInfo create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.pQueueCreateInfos = queue_create_infos.data();
  create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
  create_info.pEnabledFeatures = &device_features;
  create_info.enabledExtensionCount = static_cast<uint32_t>(vapi::cVulkanDeviceExtensions.size());
  create_info.ppEnabledExtensionNames = vapi::cVulkanDeviceExtensions.data();

  if (vkCreateDevice(physical_device_, &create_info, nullptr, &logical_device_) != VK_SUCCESS)
  {
    LOG_ERROR("Failed to create logical device");
    return;
  }

  // Queues

  vkGetDeviceQueue(logical_device_, graphics_queue_family_index_, 0, &graphics_queue_);
  vkGetDeviceQueue(logical_device_, present_queue_family_index_, 0, &present_queue_);
  vkGetDeviceQueue(logical_device_, transfer_queue_family_index_, 0, &transfer_queue_);
  vkGetDeviceQueue(logical_device_, compute_queue_family_index_, 0, &compute_queue_);

  // Memory allocator

  VmaAllocatorCreateInfo vma_allocator_create_info = {};
  vma_allocator_create_info.physicalDevice = physical_device_;
  vma_allocator_create_info.device = logical_device_;
  vma_allocator_create_info.instance = instance;

  vmaCreateAllocator(&vma_allocator_create_info, &allocator_);
}

VulkanDevice::~VulkanDevice()
{
  vkDestroyDevice(logical_device_, nullptr);
}

VulkanBufferInterface* VulkanDevice::CreateBuffer(
  size_t size, BufferUsageFlags buffer_usage_flags, MemoryUsage memory_usage)
{
  VkBufferCreateInfo buffer_create_info {};
  buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffer_create_info.size = size;
  buffer_create_info.usage = GetBufferUsageFlagBits(buffer_usage_flags);
  buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VmaAllocationCreateInfo allocation_create_info {};
  allocation_create_info.requiredFlags = GetVmaMemoryUsage(memory_usage);

  VkBuffer buffer;
  VmaAllocation allocation;
  vmaCreateBuffer(allocator_, &buffer_create_info, &allocation_create_info, &buffer, &allocation, nullptr);

  return new VulkanBufferInterface(buffer, allocation, allocator_);
}
} // namespace plex::graphics