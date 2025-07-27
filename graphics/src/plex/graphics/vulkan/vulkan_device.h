#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_DEVICE_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_DEVICE_H

#include <string>

#include "plex/graphics/vulkan/api/vulkan_api.h"
#include "plex/graphics/vulkan/vulkan_buffer.h"
#include "plex/graphics/vulkan/vulkan_surface.h"

namespace plex::graphics
{
class VulkanDevice
{
public:
  VulkanDevice(VkInstance instance, VkSurfaceKHR surface);

  ~VulkanDevice();

  VulkanBufferInterface* CreateBuffer(size_t size, BufferUsageFlags buffer_usage_flags, MemoryUsage memory_usage);

  // clang-format off

  [[nodiscard]] VkQueue GetGraphicsQueue() const noexcept { return graphics_queue_; }
  [[nodiscard]] VkQueue GetTransferQueue() const noexcept { return transfer_queue_; }
  [[nodiscard]] VkQueue GetPresentQueue() const noexcept { return present_queue_; }
  [[nodiscard]] VkQueue GetComputeQueue() const noexcept { return compute_queue_; }

  [[nodiscard]] uint32_t GetGraphicsQueueFamilyIndex() const noexcept { return graphics_queue_family_index_; }
  [[nodiscard]] uint32_t GetTransferQueueFamilyIndex() const noexcept { return transfer_queue_family_index_; }
  [[nodiscard]] uint32_t GetPresentQueueFamilyIndex() const noexcept { return present_queue_family_index_; }
  [[nodiscard]] uint32_t GetComputeQueueFamilyIndex() const noexcept { return compute_queue_family_index_; }

  // clang-format on

  [[nodiscard]] VkDevice GetHandle() const noexcept
  {
    return logical_device_;
  }

  [[nodiscard]] VkPhysicalDevice GetPhysicalHandle() const noexcept
  {
    return physical_device_;
  }

private:
  VkPhysicalDevice physical_device_;
  VkDevice logical_device_;

  VmaAllocator allocator_;

  VkQueue graphics_queue_;
  VkQueue transfer_queue_;
  VkQueue present_queue_;
  VkQueue compute_queue_;

  uint32_t graphics_queue_family_index_;
  uint32_t transfer_queue_family_index_;
  uint32_t present_queue_family_index_;
  uint32_t compute_queue_family_index_;
};
} // namespace plex::graphics

#endif
