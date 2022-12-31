#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_RENDERER_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_RENDERER_H

#include <string>

#include "plex/graphics/renderer.h"
#include "plex/graphics/vulkan/vulkan_device.h"
#include "plex/graphics/vulkan/vulkan_instance.h"
#include "plex/graphics/vulkan/vulkan_surface.h"
#include "plex/graphics/vulkan/vulkan_swapchain.h"
#include "plex/graphics/vulkan_capable_window.h"

namespace plex::graphics
{
class VulkanRenderer : public Renderer
{
public:
  VulkanRenderer(VulkanCapableWindow* window,
    const std::string& application_name,
    DebugLevel debug_level,
    PresentMode present_mode,
    uint32_t width,
    uint32_t height,
    uint32_t image_count);

  ~VulkanRenderer() override;

  Frame* AquireNextFrame() override;

  void Render(CommandBuffer* command_buffer);

  void Present() override;

private:
  VulkanInstance instance_;
  VulkanSurface surface_;
  VulkanDevice device_;
  VulkanSwapchain swapchain_;

  uint32_t frame_index_;

  std::vector<VulkanFrame> frames_;
};

} // namespace plex::graphics

#endif
