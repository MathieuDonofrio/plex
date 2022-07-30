#include "vulkan_format_util.h"

#include <array>
#include <utility>

namespace genebits::engine
{

consteval std::array<uint8_t, 131> GenerateLUT() noexcept
{
  const auto pair_count = 18;
  const std::pair<VkFormat, VkFormat> ranges[pair_count] = {
    { VK_FORMAT_R4G4B4A4_UNORM_PACK16, VK_FORMAT_A1R5G5B5_UNORM_PACK16 },
    { VK_FORMAT_R8_UNORM, VK_FORMAT_R8_SRGB },
    { VK_FORMAT_R8G8_UNORM, VK_FORMAT_R8G8_SRGB },
    { VK_FORMAT_R8G8B8_UNORM, VK_FORMAT_B8G8R8_SRGB },
    { VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_A2B10G10R10_SINT_PACK32 },
    { VK_FORMAT_R16_UNORM, VK_FORMAT_R16_SFLOAT },
    { VK_FORMAT_R16G16_UNORM, VK_FORMAT_R16G16_SFLOAT },
    { VK_FORMAT_R16G16B16_UNORM, VK_FORMAT_R16G16B16_SFLOAT },
    { VK_FORMAT_R16G16B16A16_UNORM, VK_FORMAT_R16G16B16A16_SFLOAT },
    { VK_FORMAT_R32_UINT, VK_FORMAT_R32_SFLOAT },
    { VK_FORMAT_R32G32_UINT, VK_FORMAT_R32G32_SFLOAT },
    { VK_FORMAT_R32G32B32_UINT, VK_FORMAT_R32G32B32_SFLOAT },
    { VK_FORMAT_R32G32B32A32_UINT, VK_FORMAT_R32G32B32A32_SFLOAT },
    { VK_FORMAT_R64_UINT, VK_FORMAT_R64_SFLOAT },
    { VK_FORMAT_R64G64_UINT, VK_FORMAT_R64G64_SFLOAT },
    { VK_FORMAT_R64G64B64_UINT, VK_FORMAT_R64G64B64_SFLOAT },
    { VK_FORMAT_R64G64B64A64_UINT, VK_FORMAT_R64G64B64A64_SFLOAT },
    { VK_FORMAT_B10G11R11_UFLOAT_PACK32, VK_FORMAT_E5B9G9R9_UFLOAT_PACK32 },
  };

  const std::size_t sizes[pair_count] = { 2, 1, 2, 3, 4, 2, 4, 6, 8, 4, 8, 12, 16, 8, 16, 24, 32, 4 };

  std::array<uint8_t, 131> lut {};

  lut[VK_FORMAT_UNDEFINED] = 0;
  lut[VK_FORMAT_R8_UNORM] = 1;
  lut[VK_FORMAT_D16_UNORM] = 2;
  lut[VK_FORMAT_X8_D24_UNORM_PACK32] = 4;
  lut[VK_FORMAT_D32_SFLOAT] = 4;
  lut[VK_FORMAT_S8_UINT] = 1;
  lut[VK_FORMAT_D16_UNORM_S8_UINT] = 3;
  lut[VK_FORMAT_D24_UNORM_S8_UINT] = 5;
  lut[VK_FORMAT_D32_SFLOAT_S8_UINT] = 9;

  for (int i = 0; i < pair_count; ++i)
  {
    for (int j = ranges[i].first; j <= ranges[i].second; ++j)
    {
      lut[j] = sizes[i];
    }
  }

  return lut;
}

static constexpr auto format_lut = GenerateLUT();

constexpr std::size_t GetFormatSize(VkFormat format) noexcept
{
  const auto index = static_cast<std::size_t>(format);
  return format_lut[index];
}

} // namespace genebit/////////////////////s::engine