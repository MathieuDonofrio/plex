#ifndef PLEX_GRAPHICS_VULKAN_CAPABLE_WINDOW_H
#define PLEX_GRAPHICS_VULKAN_CAPABLE_WINDOW_H

#include <vector>

#include <vulkan/vulkan_core.h>

namespace plex
{
///
/// Interface for windows that are vulkan capable.
///
/// Provides methods for initializing window with vulkan.
///
class VulkanCapableWindow
{
public:
  ///
  /// Creates a Vulkan surface for the window's drawable area.
  ///
  /// @param[in] instance Vulkan instance of the application.
  ///
  /// @Return Vulkan surface
  ///
  [[nodiscard]] virtual VkSurfaceKHR CreateWindowSurface(VkInstance instance) = 0;

  ///
  /// Returns the names of vulkan instance extensions required by the window API to create
  /// vulkan surfaces for the window.
  ///
  /// @return Vulkan instance extensions required for the window.
  ///
  [[nodiscard]] virtual std::vector<const char*> GetRequiredInstanceExtensions() = 0;

  ///
  /// Checks whether or not a specific queue family of a physical device supports image
  /// presentation.
  ///
  /// @param[in] instance Vulkan instance
  /// @param[in] physical_device Vulkan physical device
  /// @param[in] queue_family_index Queue family index
  ///
  /// @return True if queue family of physical device supports image presentation, false otherwise.
  ///
  virtual bool GetPhysicalDevicePresentationSupport(
    VkInstance instance, VkPhysicalDevice physical_device, uint32_t queue_family_index) = 0;
};
} // namespace plex

#endif
