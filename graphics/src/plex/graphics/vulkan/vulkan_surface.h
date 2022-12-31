#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_SURFACE_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_SURFACE_H

#include <vector>

#include "plex/graphics/vulkan/api/vulkan_api.h"
#include "plex/graphics/vulkan_capable_window.h"

namespace plex::graphics
{
class VulkanSurface
{
public:
  VulkanSurface(VulkanCapableWindow* window, VkInstance instance);

  ~VulkanSurface();

  [[nodiscard]] std::vector<VkSurfaceFormatKHR> GetSurfaceFormats(VkPhysicalDevice device) const;

  [[nodiscard]] std::vector<VkPresentModeKHR> GetPresentModes(VkPhysicalDevice device) const;

  [[nodiscard]] VkSurfaceCapabilitiesKHR GetSurfaceCapabilities(VkPhysicalDevice device) const;

  [[nodiscard]] VkSurfaceKHR GetHandle() const
  {
    return surface_;
  }

private:
  VkSurfaceKHR surface_;
  VkInstance instance_;
};

} // namespace plex::graphics

#endif
