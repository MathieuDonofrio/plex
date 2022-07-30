#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_BUFFER_VIEW_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_BUFFER_VIEW_H

#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include <memory>

#include "vulkan/vulkan_core.h"

namespace genebits::engine
{

class VulkanBufferView
{
public:
  VulkanBufferView() = default;
  VulkanBufferView(std::shared_ptr<VulkanDevice> device, VkFormat format, VkBuffer buffer_handle);

  ~VulkanBufferView();

private:
  std::shared_ptr<VulkanDevice> device_;
  VkBufferView buffer_view_handle_ { VK_NULL_HANDLE };
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_BUFFER_VIEW_H
