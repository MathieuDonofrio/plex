#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_VERTEX_BUFFER_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_VERTEX_BUFFER_H

#include "genebits/engine/graphics/vulkan/vulkan_device.h"

#include <cstdint>
#include <utility>

#include "vulkan/vulkan_core.h"

namespace genebits::engine
{

/*
 pseudo-code:
  VBO bvo;
  vbo->AddAttribute(
    Type: Float,
    offset: 0,
    stride: sizeof(float) * 3,
  )

*/

enum class VertexAttributeType
{
  Float,
  FloatVector2,
  FloatVector3,
  FloatVector4,
  Double,
  DoubleVector2,
  DoubleVector3,
  DoubleVector4,
  Int,
  IntVector2,
  IntVector3,
  IntVector4,
  UInt,
  UIntVector2,
  UIntVector3,
  UIntVector4,
  Bool,
  BoolVector2,
  BoolVector3,
  BoolVector4,
};

enum class InputRate
{
  Vertex,
  Instance,
};

class VulkanVertexBuffer
{
  friend class VulkanShaderProgram;

public:
  VulkanVertexBuffer(std::shared_ptr<VulkanDevice> device,
    uint32_t binding_index, // NOLINT(bugprone-easily-swappable-parameters)
    std::size_t stride, // NOLINT(bugprone-easily-swappable-parameters)
    InputRate input_rate = InputRate::Vertex)
    : device_(std::move(device)), binding_index_(binding_index), input_rate_(input_rate), stride_(stride)
  {
    auto p_handle = device_->GetPhysicalDeviceHandle();
  }

  void AddAttribute(VertexAttributeType type, uint32_t offset)
  {
    const auto location_index = attribute_descriptions_.size();
    auto format = ConvertAttributeType(type);
    //attribute_descriptions_.emplace_back(binding_index_, location_index, format, offset);
  }

private:

  VkFormat ConvertAttributeType(VertexAttributeType type)
  {
    switch (type)
    {
    case VertexAttributeType::Float: return VK_FORMAT_R32_SFLOAT;
    case VertexAttributeType::FloatVector2: return VK_FORMAT_R32G32_SFLOAT;
    case VertexAttributeType::FloatVector3: return VK_FORMAT_R32G32B32_SFLOAT;
    case VertexAttributeType::FloatVector4: return VK_FORMAT_R32G32B32A32_SFLOAT;
    case VertexAttributeType::Double: return VK_FORMAT_R64_SFLOAT;
    case VertexAttributeType::DoubleVector2: return VK_FORMAT_R64G64_SFLOAT;
    case VertexAttributeType::DoubleVector3: return VK_FORMAT_R64G64B64_SFLOAT;
    case VertexAttributeType::DoubleVector4: return VK_FORMAT_R64G64B64A64_SFLOAT;
    case VertexAttributeType::Int: return VK_FORMAT_R32_SINT;
    case VertexAttributeType::IntVector2: return VK_FORMAT_R32G32_SINT;
    case VertexAttributeType::IntVector3: return VK_FORMAT_R32G32B32_SINT;
    case VertexAttributeType::IntVector4: return VK_FORMAT_R32G32B32A32_SINT;
    case VertexAttributeType::UInt: return VK_FORMAT_R32_UINT;
    case VertexAttributeType::UIntVector2: return VK_FORMAT_R32G32_UINT;
    case VertexAttributeType::UIntVector3: return VK_FORMAT_R32G32B32_UINT;
    case VertexAttributeType::UIntVector4: return VK_FORMAT_R32G32B32A32_UINT;
    case VertexAttributeType::Bool: return VK_FORMAT_R32_SINT;
    case VertexAttributeType::BoolVector2: return VK_FORMAT_R32G32_SINT;
    case VertexAttributeType::BoolVector3: return VK_FORMAT_R32G32B32_SINT;
    case VertexAttributeType::BoolVector4: return VK_FORMAT_R32G32B32A32_SINT;
    default: return VK_FORMAT_UNDEFINED;
    }
  }

  [[nodiscard]] static VkVertexInputRate ConvertInputRate(InputRate rate) noexcept
  {
    switch (rate)
    {
    case InputRate::Vertex: return VK_VERTEX_INPUT_RATE_VERTEX;
    case InputRate::Instance: return VK_VERTEX_INPUT_RATE_INSTANCE;
    }
  }

  [[nodiscard]] VkVertexInputBindingDescription GetBindingDescription() const noexcept
  {
    VkVertexInputBindingDescription binding_description = {};
    binding_description.binding = binding_index_;
    binding_description.stride = stride_;
    binding_description.inputRate = ConvertInputRate(input_rate_);
    return binding_description;
  }

  [[nodiscard]] const std::vector<VkVertexInputAttributeDescription>& GetVertexInputBindingDescriptions() const noexcept
  {
    return attribute_descriptions_;
  }

private:
  std::shared_ptr<VulkanDevice> device_;
  InputRate input_rate_;
  uint32_t binding_index_;
  std::size_t stride_;
  std::vector<VkVertexInputAttributeDescription> attribute_descriptions_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_VERTEX_BUFFER_H
