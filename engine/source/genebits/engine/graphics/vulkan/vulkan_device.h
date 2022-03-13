#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_DEVICE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_DEVICE_H

#include "genebits/engine/graphics/device.h"
#include "vulkan_instance.h"
#include "vulkan_surface.h"

#include <memory>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{
struct VulkanQueueFamilyIndices
{
  uint32_t graphics_family_index;
  uint32_t present_family_index;
};

class VulkanDevice : public Device
{
public:
  VulkanDevice(std::shared_ptr<VulkanInstance> instance, std::shared_ptr<VulkanSurface> surface);

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

private:
  bool PickPhysicalDevice(
    const std::shared_ptr<VulkanInstance>& instance, const std::shared_ptr<VulkanSurface>& surface);

  bool Initialize(const std::shared_ptr<VulkanSurface>& surface);

  static bool IsPhysicalDeviceSupported(VkPhysicalDevice physical_device, VkSurfaceKHR surface);

  static bool FindQueueFamilies(
    VkPhysicalDevice physical_device, VkSurfaceKHR surface, VulkanQueueFamilyIndices* indices);

  static bool IsExtensionSupported(VkPhysicalDevice physical_device, const std::string& extension_name);

  static uint32_t ComputePhysicalDeviceScore(VkPhysicalDevice physical_device);

  static std::vector<VkPhysicalDevice> GetAvailablePhysicalDevices(VkInstance instance);

private:
  VkPhysicalDevice physical_device_;
  VkDevice logical_device_;

  VulkanQueueFamilyIndices queue_family_indices_;

  VkQueue graphics_queue_;
  VkQueue present_queue_;
};

} // namespace genebits::engine
#endif
