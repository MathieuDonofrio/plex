#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_SWAP_CHAIN_SUPPORT_DETAILS_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_SWAP_CHAIN_SUPPORT_DETAILS_H

#include <vector>

#include "vulkan/vulkan_core.h"

namespace genebits::engine
{

struct VulkanSwapChainSupportDetails
{
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> present_modes;
};

} // namespace genebits::engine

#endif
