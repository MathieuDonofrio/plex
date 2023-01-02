#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_RENDERER_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_RENDERER_H

#include <array>
#include <filesystem>
#include <string>

#include "plex/graphics/renderer.h"
#include "plex/graphics/vulkan/vulkan_command_buffer.h"
#include "plex/graphics/vulkan/vulkan_device.h"
#include "plex/graphics/vulkan/vulkan_instance.h"
#include "plex/graphics/vulkan/vulkan_shader_compiler.h"
#include "plex/graphics/vulkan/vulkan_surface.h"
#include "plex/graphics/vulkan/vulkan_swapchain.h"
#include "plex/graphics/vulkan_capable_window.h"

namespace plex::graphics
{
struct FrameData
{
  VkSemaphore image_available_semaphore;
  VkSemaphore render_finished_semaphore;
  VkFence fence;
  VkCommandPool command_pool;

  VulkanCommandBuffer primary_command_buffer;
};

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

  CommandBuffer* AcquireNextFrame() override;

  void Render() override;

  void Present() override;

  [[nodiscard]] std::unique_ptr<Material> CreateMaterial(const MaterialCreateInfo& create_info) override;

  [[nodiscard]] std::optional<std::unique_ptr<Shader>> CreateShader(
    const std::filesystem::path& source_path, ShaderType type) override;

  [[nodiscard]] size_t GetFrameCount() const noexcept
  {
    return swapchain_.GetImageCount();
  }

private:
  VulkanInstance instance_;
  VulkanSurface surface_;
  VulkanDevice device_;
  VulkanSwapchain swapchain_;

  std::array<FrameData, 3> frames_;

  uint32_t current_frame_index_;
  uint32_t current_image_index_;

  VulkanShaderCompiler shader_compiler_;

  VkRenderPass render_pass_;
};

} // namespace plex::graphics

#endif
