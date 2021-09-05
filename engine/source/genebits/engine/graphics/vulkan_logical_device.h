#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_LOGICAL_DEVICE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_LOGICAL_DEVICE_H

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanLogicalDevice
{
public:
  VulkanLogicalDevice();

private:
  VkDevice instance_device_;
  VkPhysicalDevice physical_device_;

  VkQueue graphics_queue_;
  VkQueue present_queue_;
};

} // namespace genebits::engine

#endif
