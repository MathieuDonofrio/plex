#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_SPV_BINARY_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_SPV_BINARY_H

#include <string>
#include <vector>
#include <utility>

namespace plex::graphics
{

class VulkanSpvBinary
{
public:
  VulkanSpvBinary() = default;
  VulkanSpvBinary(const VulkanSpvBinary&) = delete;
  VulkanSpvBinary(VulkanSpvBinary&&) = default;
  VulkanSpvBinary& operator=(const VulkanSpvBinary&) = delete;
  VulkanSpvBinary& operator=(VulkanSpvBinary&&) = default;
  ~VulkanSpvBinary() = default;

  VulkanSpvBinary(std::vector<uint32_t>&& data, const std::string& source_filename)
    : data_(std::move(data)), source_filename_(source_filename)
  {}

  [[nodiscard]] constexpr std::size_t GetSize() const
  {
    return data_.size();
  }

  [[nodiscard]] constexpr const uint32_t* GetData() const
  {
    return data_.data();
  }

  [[nodiscard]] const std::string& GetSourceFilename() const
  {
    return source_filename_;
  }

private:
  std::vector<uint32_t> data_;
  std::string source_filename_;
};
} // namespace plex::graphics

#endif // PLEX_GRAPHICS_VULKAN_VULKAN_SPV_BINARY_H