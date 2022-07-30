#ifndef PLEX_GRAPHICS_VULKAN_DEVICE_H
#define PLEX_GRAPHICS_VULKAN_DEVICE_H

#include "plex/debug/logging.h"
#include "plex/graphics/device.h"
#include "plex/graphics/vulkan/swapchain_image_usage.h"
#include "plex/graphics/vulkan/vulkan_instance.h"
#include "plex/graphics/vulkan/vulkan_surface.h"

#include <memory>

#include <vk_mem_alloc.h>
#include <vulkan/vulkan_core.h>

namespace plex
{
struct VulkanQueueFamilyIndices
{
  uint32_t graphics_family_index;
  uint32_t present_family_index;
  uint32_t compute_family_index;

  [[nodiscard]] bool AreAllFamilySame() const noexcept
  {
    return graphics_family_index == present_family_index && graphics_family_index == compute_family_index;
  }
};

class VulkanDevice : public Device
{
public:
  VulkanDevice(std::shared_ptr<VulkanInstance> instance,
    std::shared_ptr<VulkanSurface> surface,
    SwapchainImageUsage swapchain_image_usage = SwapchainImageUsage::ColorAttachment);

  VulkanDevice(const VulkanDevice&) = delete;
  VulkanDevice& operator=(const VulkanDevice&) = delete;
  VulkanDevice(VulkanDevice&&) = delete;
  VulkanDevice& operator=(VulkanDevice&&) = delete;

  ~VulkanDevice() override;

  [[nodiscard]] const VulkanQueueFamilyIndices& GetQueueFamilyIndices() const noexcept
  {
    return queue_family_indices_;
  }

  [[nodiscard]] VkPhysicalDevice GetPhysicalDeviceHandle() const noexcept
  {
    return physical_device_;
  }

  [[nodiscard]] VkDevice GetHandle() const noexcept
  {
    return logical_device_;
  }

  [[nodiscard]] const VmaAllocator& GetAllocator() const noexcept
  {
    return vma_allocator_;
  }

  [[nodiscard]] VkQueue GetComputeQueue() const noexcept
  {
    return compute_queue_;
  }

  [[nodiscard]] VkQueue GetPresentQueue() const noexcept
  {
    return present_queue_;
  }

  VkFormat FindSupportedFormat(
    const std::vector<VkFormat>& format_candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
  {
    for (VkFormat format : format_candidates)
    {
      VkFormatProperties format_properties;
      vkGetPhysicalDeviceFormatProperties(physical_device_, format, &format_properties);

      if (tiling == VK_IMAGE_TILING_LINEAR && (format_properties.linearTilingFeatures & features) == features
          || tiling == VK_IMAGE_TILING_OPTIMAL && (format_properties.optimalTilingFeatures & features) == features)
      {
        return format;
      }
    }

    LOG_ERROR("Failed to find supported format");
    return VK_FORMAT_UNDEFINED;
  }

  uint32_t FindMemoryTypeIndex(uint32_t typeFilter, VkMemoryPropertyFlags properties)
  {
    VkPhysicalDeviceMemoryProperties memory_properties;
    vkGetPhysicalDeviceMemoryProperties(physical_device_, &memory_properties);

    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)
    {
      if (!(typeFilter & (1 << i))) continue;
      if ((memory_properties.memoryTypes[i].propertyFlags & properties) != properties) continue;
      return i;
    }

    return UINT32_MAX;
  }

private:
  template<typename T>
  T GetFunctionPointer(const char* name) const noexcept
  {
    return reinterpret_cast<T>(vkGetInstanceProcAddr(instance_->GetHandle(), name));
  }

  [[nodiscard]] static VkImageUsageFlags ConvertSwapchainUsage(SwapchainImageUsage usage) noexcept
  {
    switch (usage)
    {
    case SwapchainImageUsage::StorageImage: return VK_IMAGE_USAGE_STORAGE_BIT;
    case SwapchainImageUsage::ColorAttachment: return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    case SwapchainImageUsage::TransferDestination: return VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    default:
    {
      LOG_ERROR("Unknown swapchain image usage");
      return {};
    }
    }
  }

  bool PickPhysicalDevice(const std::shared_ptr<VulkanInstance>& instance,
    const std::shared_ptr<VulkanSurface>& surface,
    const VkImageUsageFlags swapchain_image_usage);

  bool Initialize(const std::shared_ptr<VulkanSurface>& surface);

  bool InitializeVMA(const std::shared_ptr<VulkanInstance>& instance);

  static bool IsPhysicalDeviceSupported(
    VkPhysicalDevice physical_device, VkSurfaceKHR surface, VkImageUsageFlags usage);

  static bool FindQueueFamilies(
    VkPhysicalDevice physical_device, VkSurfaceKHR surface, VulkanQueueFamilyIndices* indices);

  static bool IsExtensionSupported(VkPhysicalDevice physical_device, const std::string& extension_name);

  static uint32_t ComputePhysicalDeviceScore(VkPhysicalDevice physical_device);

  static std::vector<VkPhysicalDevice> GetAvailablePhysicalDevices(VkInstance instance);

  [[nodiscard]] VkPhysicalDeviceLimits GetPhysicalDeviceLimits() const;

private:
  VkPhysicalDevice physical_device_;
  VkDevice logical_device_;
  VmaAllocator vma_allocator_;
  std::shared_ptr<VulkanInstance> instance_;

  VulkanQueueFamilyIndices queue_family_indices_;

  VkQueue graphics_queue_;
  VkQueue present_queue_;
  VkQueue compute_queue_;

  std::array<uint32_t, 3> max_compute_work_group_count_;
  std::array<uint32_t, 3> max_compute_work_group_size_;
  uint32_t max_compute_work_group_invocations_;
};

} // namespace plex
#endif
