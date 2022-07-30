#include "render_pass_2.h"

namespace genebits::engine
{

RenderPass2::RenderPass2(std::shared_ptr<VulkanDevice> device) : device_(std::move(device))
{
}

std::shared_ptr<VulkanSubPass> RenderPass2::AddSubPass(std::shared_ptr<VulkanShaderProgram> shader_program)
{
  auto sub_pass = std::make_shared<VulkanSubPass>(std::move(shader_program));
  sub_passes_.emplace_back(sub_pass);
  return sub_pass;
}

} // namespace genebits::engine