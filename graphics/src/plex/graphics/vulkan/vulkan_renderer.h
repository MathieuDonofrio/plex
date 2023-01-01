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

  void Render(Frame* frame, CommandBuffer* command_buffer) override;

  void Present(Frame* frame) override;

  [[nodiscard]] size_t GetFrameCount() const noexcept
  {
    return swapchain_.GetImageCount();
  }

private:
  VulkanInstance instance_;
  VulkanSurface surface_;
  VulkanDevice device_;
  VulkanSwapchain swapchain_;

  VulkanFrame* frames_; // TODO use FixedVector when implemented
  uint32_t frame_index_;
};

} // namespace plex::graphics

#endif
