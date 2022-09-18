#ifndef PLEX_ENGINE_GRAPHICS_VULKAN_API_HELPERS_H
#define PLEX_ENGINE_GRAPHICS_VULKAN_API_HELPERS_H

#include "vulkan_types.h"

#include <utility>

namespace plex::graphics::vkapi {

template<typename T>
struct VulkanResultWithValue
{
  VkResult result {};
  T value {};

  VulkanResultWithValue() = default;
  VulkanResultWithValue(T&& value) : value(std::move(value)) {}
  VulkanResultWithValue(VkResult result) : result(result) {}

  operator bool() const
  {
    return result == VK_SUCCESS;
  }
};

struct VulkanResult
{
  VkResult result;

  VulkanResult() = default;
  VulkanResult(VkResult result) : result(result) {}

  operator bool() const
  {
    return result == VK_SUCCESS;
  }
};

struct VulkanFunctionPointer
{
  void (*callback)() {};

  VulkanFunctionPointer(void (*callback)()) : callback(callback) {}

  operator bool() const
  {
    return callback != nullptr;
  }
};

}

#endif // GENEBITS_VULKAN_API_HELPERS_H
