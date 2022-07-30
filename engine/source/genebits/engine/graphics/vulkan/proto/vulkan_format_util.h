#ifndef GENEBITS_VULKAN_FORMAT_UTIL_H
#define GENEBITS_VULKAN_FORMAT_UTIL_H

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{
  constexpr size_t GetFormatSize(VkFormat format) noexcept;
}

#endif // GENEBITS_VULKAN_FORMAT_UTIL_H
