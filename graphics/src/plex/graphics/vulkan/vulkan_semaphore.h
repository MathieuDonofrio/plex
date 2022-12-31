#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_SEMAPHORE_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_SEMAPHORE_H

#include "plex/graphics/semaphore.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"

namespace plex::graphics
{

class VulkanSemaphore : public Semaphore
{
public:
  VulkanSemaphore(VkDevice device);
  ~VulkanSemaphore() override;

  void Wait() override;

  [[nodiscard]] VkSemaphore GetHandle() const noexcept
  {
    return semaphore_;
  }

private:
  VkSemaphore semaphore_;
  VkDevice device_;
};
} // namespace plex::graphics

#endif