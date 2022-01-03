#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_QUEUE_FAMILY_INDICES_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_QUEUE_FAMILY_INDICES_H

#include "genebits/engine/debug/assertion.h"

#include <optional>

namespace genebits::engine
{

class VulkanQueueFamilyIndices
{
public:
  VulkanQueueFamilyIndices() = default;
  VulkanQueueFamilyIndices(std::optional<uint32_t> graphics_family_index, std::optional<uint32_t> present_family_index)
    : graphics_family_index_(graphics_family_index),
      present_family_index_(present_family_index), queue_family_indices_ {}
  {}

  [[nodiscard]] bool IsGraphicsAndPresentFamilyIndexPresent() const noexcept
  {
    return graphics_family_index_.has_value() && present_family_index_.has_value();
  }

  [[nodiscard]] uint32_t GetGraphicsFamilyIndex() const
  {
    ASSERT(graphics_family_index_.has_value(), "Tried to get value of graphics family index but none is present");
    return graphics_family_index_.value();
  }

  [[nodiscard]] uint32_t GetPresentFamilyIndex() const
  {
    ASSERT(present_family_index_.has_value(), "Tried to get value of present family index but none is present");
    return present_family_index_.value();
  }

  [[nodiscard]] bool SameFamilies() const noexcept
  {
    ASSERT(IsGraphicsAndPresentFamilyIndexPresent(), "Missing family index to compare families");
    return graphics_family_index_ == present_family_index_;
  }

  [[nodiscard]] const std::array<uint32_t, 2>& GetFamilyIndicesArray()
  {
    if (queue_family_indices_.empty())
    {
      queue_family_indices_ = { GetGraphicsFamilyIndex(), GetPresentFamilyIndex() };
    }
    return queue_family_indices_;
  }

private:
  std::optional<uint32_t> graphics_family_index_;
  std::optional<uint32_t> present_family_index_;
  std::array<uint32_t, 2> queue_family_indices_;
};

} // namespace genebits::engine

#endif
