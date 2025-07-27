#include "plex/graphics/vulkan/vulkan_surface.h"

namespace plex::graphics
{
VulkanSurface::VulkanSurface(VulkanCapableWindow* window, VkInstance instance) : instance_(instance)
{
  surface_ = window->CreateWindowSurface(instance);
}

VulkanSurface::~VulkanSurface()
{
  vkDestroySurfaceKHR(instance_, surface_, nullptr);
}

std::vector<VkSurfaceFormatKHR> VulkanSurface::GetSurfaceFormats(VkPhysicalDevice device) const
{
  uint32_t format_count = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &format_count, nullptr);
  std::vector<VkSurfaceFormatKHR> formats(format_count);
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &format_count, formats.data());
  return formats;
}

std::vector<VkPresentModeKHR> VulkanSurface::GetPresentModes(VkPhysicalDevice device) const
{
  uint32_t present_mode_count = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &present_mode_count, nullptr);
  std::vector<VkPresentModeKHR> present_modes(present_mode_count);
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &present_mode_count, present_modes.data());
  return present_modes;
}

VkSurfaceCapabilitiesKHR VulkanSurface::GetSurfaceCapabilities(VkPhysicalDevice device) const
{
  VkSurfaceCapabilitiesKHR surface_capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_, &surface_capabilities);
  return surface_capabilities;
}

} // namespace plex::graphics
