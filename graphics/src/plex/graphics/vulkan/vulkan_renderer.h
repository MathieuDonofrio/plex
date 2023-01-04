#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_RENDERER_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_RENDERER_H

#include <array>
#include <filesystem>
#include <string>

#include "plex/graphics/renderer.h"
#include "plex/graphics/spirv/shader_compiler.h"
#include "plex/graphics/vulkan/vulkan_command_buffer.h"
#include "plex/graphics/vulkan/vulkan_device.h"
#include "plex/graphics/vulkan/vulkan_instance.h"
#include "plex/graphics/vulkan/vulkan_surface.h"
#include "plex/graphics/vulkan/vulkan_swapchain.h"
#include "plex/graphics/vulkan_capable_window.h"

namespace plex::graphics
{
struct FrameLocalData
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
  VulkanRenderer(const RendererCreateInfo& create_info);

  ~VulkanRenderer() override;

  CommandBuffer* AcquireNextFrame() override;
  void Render() override;
  void Present() override;

  void SubmitImmediate(std::function<void(CommandBuffer*)> func) override;

  void WaitIdle() override;

  [[nodiscard]] std::unique_ptr<Material> CreateMaterial(const MaterialCreateInfo& create_info) override;
  [[nodiscard]] std::unique_ptr<Shader> CreateShader(
    const std::string& source, const std::filesystem::path& source_path, ShaderType type) override;

  [[nodiscard]] size_t GetFrameCount() const noexcept
  {
    return swapchain_.GetImageCount();
  }

private:
  pbi::Buffer CreateBuffer(size_t size, BufferUsageFlags usage, MemoryPropertyFlags properties) override;

  void WindowFramebufferResizeCallback(const WindowFramebufferResizeEvent&);

private:
  Window* window_;

  VulkanInstance instance_;
  VulkanSurface surface_;
  VulkanDevice device_;
  VulkanSwapchain swapchain_;

  std::array<FrameLocalData, 3> frames_;

  uint32_t current_frame_index_;
  uint32_t current_image_index_;

  VkRenderPass render_pass_;

  VkFence submit_immediate_fence_;
  VkCommandPool command_pool_;

  VulkanCommandBuffer submit_immediate_command_buffer_;

  ShaderCompiler shader_compiler_;
};

} // namespace plex::graphics

#endif
