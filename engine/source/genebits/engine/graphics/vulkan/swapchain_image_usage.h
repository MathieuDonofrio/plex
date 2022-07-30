#ifndef GENEBITS_ENGINE_GRAPHICS_SWAPCHAIN_IMAGE_USAGE_H
#define GENEBITS_ENGINE_GRAPHICS_SWAPCHAIN_IMAGE_USAGE_H

namespace genebits::engine
{

enum class SwapchainImageUsage
{
  StorageImage,
  ColorAttachment,
  TransferDestination,
};

}

#endif // GENEBITS_ENGINE_GRAPHICS_SWAPCHAIN_IMAGE_USAGE_H
