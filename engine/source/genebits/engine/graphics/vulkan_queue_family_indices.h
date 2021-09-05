#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_QUEUE_FAMILY_INDICES_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_QUEUE_FAMILY_INDICES_H

#include <optional>

namespace genebits::engine
{

class VulkanQueueFamilyIndices
{
public:
  VulkanQueueFamilyIndices() = default;
  VulkanQueueFamilyIndices(std::optional<uint32_t> graphics_family_index, std::optional<uint32_t> present_family_index);

  [[nodiscard]] bool IsGraphicsAndPresentFamilyIndexPresent() const noexcept;

  [[nodiscard]] uint32_t GetGraphicsFamilyIndex() const;
  [[nodiscard]] uint32_t GetPresentFamilyIndex() const;

private:
  std::optional<uint32_t> graphics_family_index_;
  std::optional<uint32_t> present_family_index_;
};

} // namespace genebits::engine

#endif
