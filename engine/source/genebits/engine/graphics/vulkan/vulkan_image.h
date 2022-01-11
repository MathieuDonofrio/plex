#ifndef GENEBITS_VULKAN_IMAGE_H
#define GENEBITS_VULKAN_IMAGE_H

#include "genebits/engine/debug/logging.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include <memory>
#include <utility>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanImage
{
public:
  VulkanImage() = default;
  virtual ~VulkanImage() = default;
protected:
};

} // namespace genebits::engine

#endif // GENEBITS_VULKAN_IMAGE_H
