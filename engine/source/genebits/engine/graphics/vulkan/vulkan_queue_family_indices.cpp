#include "vulkan_queue_family_indices.h"

#include <algorithm>

#include "genebits/engine/debug/assertion.h"

namespace genebits::engine
{

VulkanQueueFamilyIndices::VulkanQueueFamilyIndices(
  std::optional<uint32_t> graphics_family_index, std::optional<uint32_t> present_family_index)
  : graphics_family_index_(graphics_family_index), present_family_index_(present_family_index)
{}

bool VulkanQueueFamilyIndices::IsGraphicsAndPresentFamilyIndexPresent() const noexcept
{
  return graphics_family_index_.has_value() && present_family_index_.has_value();
}

uint32_t VulkanQueueFamilyIndices::GetGraphicsFamilyIndex() const
{
  ASSERT(graphics_family_index_.has_value(), "Tried to get value of graphics family index but none is present");
  return graphics_family_index_.value();
}

uint32_t VulkanQueueFamilyIndices::GetPresentFamilyIndex() const
{
  ASSERT(present_family_index_.has_value(), "Tried to get value of present family index but none is present");
  return present_family_index_.value();
}

} // namespace genebits::engine