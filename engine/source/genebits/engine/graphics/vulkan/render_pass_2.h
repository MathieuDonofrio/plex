#ifndef GENEBITS_ENGINE_GRAPHICS_RENDER_PASS_2_H
#define GENEBITS_ENGINE_GRAPHICS_RENDER_PASS_2_H

#include "genebits/engine/graphics/vulkan/images/vulkan_attachment.h"
#include "genebits/engine/graphics/vulkan/resources/vulkan_resource.h"
#include "genebits/engine/graphics/vulkan/shaders/vulkan_shader_program.h"
#include "genebits/engine/graphics/vulkan/vulkan_device.h"
#include "genebits/engine/graphics/vulkan/vulkan_frame_buffer.h"
#include "genebits/engine/graphics/vulkan/vulkan_sub_pass.h"

#include <memory>
#include <vector>

namespace genebits::engine
{

class RenderPass2
{
public:
  RenderPass2(std::shared_ptr<VulkanDevice> device);

  RenderPass2(const RenderPass2&) = delete;
  RenderPass2& operator=(const RenderPass2&) = delete;

  std::shared_ptr<VulkanSubPass> AddSubPass(std::shared_ptr<VulkanShaderProgram> shader_program);

private:
  std::shared_ptr<VulkanDevice> device_;
  std::shared_ptr<VulkanFrameBuffer> frame_buffer_;
  std::vector<std::shared_ptr<VulkanSubPass>> sub_passes_;
};

} // namespace genebits::engine

#endif // GENEBITS_RENDER_PASS_2_H
