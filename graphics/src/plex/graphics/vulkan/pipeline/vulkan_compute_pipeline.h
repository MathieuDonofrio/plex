#ifndef PLEX_GRAPHICS_VULKAN_COMPUTE_PIPELINE_H
#define PLEX_GRAPHICS_VULKAN_COMPUTE_PIPELINE_H

#include "plex/graphics/vulkan/pipeline/vulkan_pipeline.h"
#include "plex/graphics/vulkan/vulkan_device.h"

#include "plex/debug/assertion.h"

#include <memory>
#include <utility>

#include <vulkan/vulkan_core.h>

namespace plex
{

enum class PushRangeStage
{
  Vertex,
  Fragment,
  Compute
};

struct PushRange
{
  PushRangeStage stage;
  uint32_t size;
  uint32_t offset;
};

class VulkanComputePipeline : public VulkanPipeline
{
public:
  const static std::size_t max_push_constant_size = 128;

  VulkanComputePipeline(std::shared_ptr<VulkanDevice> device,
    const std::vector<PushRange>& push_ranges,
    uint32_t set_count,
    const VkDescriptorSetLayout& descriptor_set_layout)
    : VulkanPipeline(), device_(std::move(device))
  {

    std::size_t total_size = 0;
    std::vector<VkPushConstantRange> vk_push_ranges;
    vk_push_ranges.reserve(push_ranges.size());
    for (const auto& push_range : push_ranges)
    {
      total_size += push_range.size;
      ASSERT(total_size <= max_push_constant_size,
        "Push constants size exceeds maximum of " + std::to_string(max_push_constant_size) + " bytes");
      vk_push_ranges.emplace_back(ConvertPushConstantStage(push_range.stage), push_range.offset, push_range.size);
    }

    VkPipelineLayoutCreateInfo layout_create_info {};
    layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layout_create_info.setLayoutCount = set_count;
    layout_create_info.pSetLayouts = &descriptor_set_layout;
    layout_create_info.pushConstantRangeCount = static_cast<uint32_t>(vk_push_ranges.size());
    layout_create_info.pPushConstantRanges = vk_push_ranges.data();

    if (vkCreatePipelineLayout(device_->GetHandle(), &layout_create_info, nullptr, &pipeline_layout_) != VK_SUCCESS)
    {
      LOG_ERROR("Failed to create pipeline layout");
      return;
    }
  }

  ~VulkanComputePipeline()
  {
    if (pipeline_layout_ != VK_NULL_HANDLE)
    {
      vkDestroyPipelineLayout(device_->GetHandle(), pipeline_layout_, nullptr);
      LOG_TRACE("Pipeline layout destroyed");
    }
    if (pipeline_handle_ != VK_NULL_HANDLE)
    {
      vkDestroyPipeline(device_->GetHandle(), pipeline_handle_, nullptr);
      LOG_TRACE("Pipeline destroyed");
    }
  }

  bool InitializePipeline(const VkPipelineShaderStageCreateInfo& shader_stage_create_info) override
  {
    VkComputePipelineCreateInfo pipeline_create_info {};
    pipeline_create_info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipeline_create_info.stage = shader_stage_create_info;
    pipeline_create_info.layout = pipeline_layout_;
    pipeline_create_info.flags = 0;

    if (vkCreateComputePipelines(
          device_->GetHandle(), VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &pipeline_handle_)
        != VK_SUCCESS)
    {
      LOG_ERROR("Failed to create compute pipeline");
      return false;
    }
    return true;
  }

  [[nodiscard]] VkPipeline GetHandle() const noexcept
  {
    return pipeline_handle_;
  }

  [[nodiscard]] VkPipelineLayout GetLayout() const noexcept
  {
    return pipeline_layout_;
  }

private:
  [[nodiscard]] VkShaderStageFlagBits ConvertPushConstantStage(PushRangeStage stage) const noexcept
  {
    switch (stage)
    {
    case PushRangeStage::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
    case PushRangeStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
    case PushRangeStage::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
    default: return VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM;
    }
  }

private:
  VkPipeline pipeline_handle_;
  VkPipelineLayout pipeline_layout_;
  std::shared_ptr<VulkanDevice> device_;
};

} // namespace plex

#endif
