#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_GRAPHICS_PIPELINE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_GRAPHICS_PIPELINE_H

#include "genebits/engine/graphics/vulkan/vulkan_device.h"
#include "genebits/engine/graphics/vulkan/pipeline/vulkan_pipeline.h"

#include "genebits/engine/debug/logging.h"

#include <memory>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

enum class PrimitiveTopology
{
  PointList,
  LineList,
  LineStrip,
  TriangleList,
  TriangleStrip
};

enum class FillMode
{
  Solid,
  Wireframe
};

enum class CullMode
{
  None,
  Front,
  Back
};

enum class FrontFace
{
  Clockwise,
  CounterClockwise
};

enum class DepthCompareOp
{
  Never,
  Less,
  Equal,
  LessOrEqual,
  Greater,
  NotEqual,
  GreaterOrEqual,
  Always
};

enum class BlendFactor
{
  Zero,
  One,
  SrcColor,
  OneMinusSrcColor,
  DstColor,
  OneMinusDstColor,
  SrcAlpha,
  OneMinusSrcAlpha,
  DstAlpha,
  OneMinusDstAlpha,
  ConstantColor,
  OneMinusConstantColor,
  ConstantAlpha,
  OneMinusConstantAlpha,
  SrcAlphaSaturate
};

enum class BlendOp
{
  Add,
  Subtract,
  ReverseSubtract,
  Min,
  Max
};

enum class LogicOp
{
  Clear,
  And,
  AndReverse,
  Copy,
  AndInverted,
  NoOp,
  Xor,
  Or,
  Nor,
  Equivalent,
  Invert,
  OrReverse,
  CopyInverted,
  OrInverted,
  Nand,
  Set
};

enum class StencilOp
{
  Keep,
  Zero,
  Replace,
  IncrementAndClamp,
  DecrementAndClamp,
  Invert,
  IncrementAndWrap,
  DecrementAndWrap
};

class VulkanGraphicsPipeline : public VulkanPipeline
{
public:
  VulkanGraphicsPipeline(std::shared_ptr<VulkanDevice> device) : device_(std::move(device)) {
    pipeline_type_ = PipelineType::Graphics;
  }

  void SetDepthTestEnabled(bool enabled)
  {
    depth_test_enabled_ = enabled;
  }

  void SetDepthWriteEnabled(bool enabled)
  {
    depth_write_enabled_ = enabled;
  }

  void SetPrimitiveTopology(PrimitiveTopology topology) noexcept
  {
    topology_ = topology;
  }

  void SetFillMode(FillMode fill_mode) noexcept
  {
    fill_mode_ = fill_mode;
  }

  void SetCullMode(CullMode cull_mode) noexcept
  {
    cull_mode_ = cull_mode;
  }

  void SetFrontFace(FrontFace front_face) noexcept
  {
    front_face_ = front_face;
  }

  void SetDepthCompareOp(DepthCompareOp depth_compare_op) noexcept
  {
    depth_compare_op_ = depth_compare_op;
  }

  void SetSourceBlendFactor(BlendFactor blend_factor) noexcept
  {
    src_blend_factor_ = blend_factor;
  }

  void SetDestinationBlendFactor(BlendFactor blend_factor) noexcept
  {
    dst_blend_factor_ = blend_factor;
  }

  void SetBlendOp(BlendOp blend_op) noexcept
  {
    blend_op_ = blend_op;
  }

  void SetBlendEnabled(bool enabled) noexcept
  {
    blend_enabled_ = enabled;
  }

  void SetSourceAlphaBlendFactor(BlendFactor blend_factor) noexcept
  {
    src_alpha_blend_factor_ = blend_factor;
  }

  void SetDestinationAlphaBlendFactor(BlendFactor blend_factor) noexcept
  {
    dst_alpha_blend_factor_ = blend_factor;
  }

  void SetAlphaBlendOp(BlendOp blend_op) noexcept
  {
    alpha_blend_op_ = blend_op;
  }

  void SetLogicOp(LogicOp logic_op) noexcept
  {
    logic_op_ = logic_op;
    LOG_WARN("Not implemented");
  }

  void SetStencilOp(StencilOp stencil_op) noexcept
  {
    stencil_op_ = stencil_op;
    LOG_WARN("Not implemented");
  }

  void SetStencilCompareOp(DepthCompareOp stencil_compare_op) noexcept
  {
    stencil_compare_op_ = stencil_compare_op;
    LOG_WARN("Not implemented");
  }

  [[nodiscard]] bool InitializePipeline(const VkPipelineShaderStageCreateInfo& shader_stage_create_info) override {
    return false;
  };

private:
  [[nodiscard]] static VkPrimitiveTopology ConvertPrimitiveTopology(PrimitiveTopology topology) noexcept
  {
    switch (topology)
    {
    case PrimitiveTopology::PointList: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    case PrimitiveTopology::LineList: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    case PrimitiveTopology::LineStrip: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
    case PrimitiveTopology::TriangleList: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    case PrimitiveTopology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
    }
  }

  [[nodiscard]] VkPipelineInputAssemblyStateCreateInfo GetInputAssemblyStateCreateInfo() const noexcept
  {
    VkPipelineInputAssemblyStateCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    create_info.topology = ConvertPrimitiveTopology(topology_);
    create_info.primitiveRestartEnable = VK_FALSE;
    return create_info;
  }

  [[nodiscard]] static VkPolygonMode ConvertFillMode(FillMode mode) noexcept
  {
    switch (mode)
    {
    case FillMode::Solid: return VK_POLYGON_MODE_FILL;
    case FillMode::Wireframe: return VK_POLYGON_MODE_LINE;
    }
  }

  [[nodiscard]] static VkCullModeFlags ConvertCullMode(CullMode mode) noexcept
  {
    switch (mode)
    {
    case CullMode::None: return VK_CULL_MODE_NONE;
    case CullMode::Front: return VK_CULL_MODE_FRONT_BIT;
    case CullMode::Back: return VK_CULL_MODE_BACK_BIT;
    }
  }

  [[nodiscard]] static VkFrontFace ConvertFrontFace(FrontFace face) noexcept
  {
    switch (face)
    {
    case FrontFace::Clockwise: return VK_FRONT_FACE_CLOCKWISE;
    case FrontFace::CounterClockwise: return VK_FRONT_FACE_COUNTER_CLOCKWISE;
    }
  }

  [[nodiscard]] VkPipelineRasterizationStateCreateInfo GetRasterizationStateCreateInfo() const noexcept
  {
    VkPipelineRasterizationStateCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    create_info.depthClampEnable = VK_FALSE;
    create_info.rasterizerDiscardEnable = VK_FALSE;
    create_info.polygonMode = ConvertFillMode(fill_mode_);
    create_info.cullMode = ConvertCullMode(cull_mode_);
    create_info.frontFace = ConvertFrontFace(front_face_);
    create_info.depthBiasEnable = VK_FALSE;
    create_info.depthBiasConstantFactor = 0.0f;
    create_info.depthBiasClamp = 0.0f;
    create_info.depthBiasSlopeFactor = 0.0f;
    create_info.lineWidth = 1.0f;
    return create_info;
  }

  [[nodiscard]] VkPipelineMultisampleStateCreateInfo GetMultisampleStateCreateInfo() const noexcept
  {
    VkPipelineMultisampleStateCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    create_info.sampleShadingEnable = VK_FALSE;
    create_info.minSampleShading = 1.0f;
    create_info.pSampleMask = nullptr;
    create_info.alphaToCoverageEnable = VK_FALSE;
    create_info.alphaToOneEnable = VK_FALSE;
    return create_info;
  }

  [[nodiscard]] static VkCompareOp ConvertDepthCompareOp(DepthCompareOp op) noexcept
  {
    switch (op)
    {
    case DepthCompareOp::Never: return VK_COMPARE_OP_NEVER;
    case DepthCompareOp::Less: return VK_COMPARE_OP_LESS;
    case DepthCompareOp::Equal: return VK_COMPARE_OP_EQUAL;
    case DepthCompareOp::LessOrEqual: return VK_COMPARE_OP_LESS_OR_EQUAL;
    case DepthCompareOp::Greater: return VK_COMPARE_OP_GREATER;
    case DepthCompareOp::NotEqual: return VK_COMPARE_OP_NOT_EQUAL;
    case DepthCompareOp::GreaterOrEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case DepthCompareOp::Always: return VK_COMPARE_OP_ALWAYS;
    }
  }

  [[nodiscard]] VkPipelineDepthStencilStateCreateInfo GetDepthStencilStateCreateInfo() const noexcept
  {
    VkPipelineDepthStencilStateCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    create_info.depthTestEnable = depth_test_enabled_;
    create_info.depthWriteEnable = depth_write_enabled_;
    create_info.depthCompareOp = ConvertDepthCompareOp(depth_compare_op_);
    create_info.depthBoundsTestEnable = VK_FALSE;
    create_info.stencilTestEnable = VK_FALSE;
    create_info.front = {};
    create_info.back = {};
    create_info.minDepthBounds = 0.0f;
    create_info.maxDepthBounds = 1.0f;
    return create_info;
  }

  [[nodiscard]] static VkBlendFactor ConvertBlendFactor(BlendFactor factor) noexcept
  {
    switch (factor)
    {
    case BlendFactor::Zero: return VK_BLEND_FACTOR_ZERO;
    case BlendFactor::One: return VK_BLEND_FACTOR_ONE;
    case BlendFactor::SrcColor: return VK_BLEND_FACTOR_SRC_COLOR;
    case BlendFactor::OneMinusSrcColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
    case BlendFactor::DstColor: return VK_BLEND_FACTOR_DST_COLOR;
    case BlendFactor::OneMinusDstColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
    case BlendFactor::SrcAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
    case BlendFactor::OneMinusSrcAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    case BlendFactor::DstAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
    case BlendFactor::OneMinusDstAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    case BlendFactor::ConstantColor: return VK_BLEND_FACTOR_CONSTANT_COLOR;
    case BlendFactor::OneMinusConstantColor: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
    case BlendFactor::ConstantAlpha: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
    case BlendFactor::OneMinusConstantAlpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
    case BlendFactor::SrcAlphaSaturate: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
    }
  }

  [[nodiscard]] static VkBlendOp ConvertBlendOp(BlendOp op) noexcept
  {
    switch (op)
    {
    case BlendOp::Add: return VK_BLEND_OP_ADD;
    case BlendOp::Subtract: return VK_BLEND_OP_SUBTRACT;
    case BlendOp::ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
    case BlendOp::Min: return VK_BLEND_OP_MIN;
    case BlendOp::Max: return VK_BLEND_OP_MAX;
    }
  }

  [[nodiscard]] VkPipelineColorBlendAttachmentState GetColorBlendAttachmentState() const noexcept
  {
    VkPipelineColorBlendAttachmentState state = {};
    state.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    state.blendEnable = blend_enabled_;
    state.srcColorBlendFactor = ConvertBlendFactor(src_blend_factor_);
    state.dstColorBlendFactor = ConvertBlendFactor(dst_blend_factor_);
    state.colorBlendOp = ConvertBlendOp(blend_op_);
    state.srcAlphaBlendFactor = ConvertBlendFactor(src_alpha_blend_factor_);
    state.dstAlphaBlendFactor = ConvertBlendFactor(dst_alpha_blend_factor_);
    state.alphaBlendOp = ConvertBlendOp(alpha_blend_op_);
    return state;
  }

private:
  std::shared_ptr<VulkanDevice> device_;

  PrimitiveTopology topology_ { PrimitiveTopology::TriangleList };
  FillMode fill_mode_ { FillMode::Solid };
  CullMode cull_mode_ { CullMode::Back };
  FrontFace front_face_ { FrontFace::CounterClockwise };

  BlendFactor src_blend_factor_ { BlendFactor::SrcAlpha };
  BlendFactor dst_blend_factor_ { BlendFactor::OneMinusSrcAlpha };
  BlendOp blend_op_ { BlendOp::Add };
  bool blend_enabled_ { true };
  BlendFactor src_alpha_blend_factor_ { BlendFactor::One };
  BlendFactor dst_alpha_blend_factor_ { BlendFactor::Zero };
  BlendOp alpha_blend_op_ { BlendOp::Add };

  LogicOp logic_op_ { LogicOp::Copy };

  StencilOp stencil_op_ { StencilOp::Keep };
  DepthCompareOp stencil_compare_op_ { DepthCompareOp::Less };
  DepthCompareOp depth_compare_op_ { DepthCompareOp::Less };
  bool depth_test_enabled_ { true };
  bool depth_write_enabled_ { true };
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_GRAPHICS_PIPELINE_H
