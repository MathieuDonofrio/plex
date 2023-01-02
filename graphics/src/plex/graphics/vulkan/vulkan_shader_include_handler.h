#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_SHADER_INCLUDE_HANDLER_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_SHADER_INCLUDE_HANDLER_H

#include <filesystem>
#include <set>
#include <memory>

#include <shaderc/shaderc.hpp>

namespace plex::graphics
{

class VulkanShaderIncludeHandler : shaderc::CompileOptions::IncluderInterface
{
public:
  void Clear()
  {
    available_include_files_.clear();
  }

  void AddIncludeFile(const std::filesystem::path& path)
  {
    available_include_files_.insert(std::filesystem::absolute(path));
  }

  bool AddIncludeDirectory(const std::filesystem::path& path, bool recursive, uint32_t max_depth = 4);

private:
  shaderc_include_result* GetInclude(const char* requested_source,
    shaderc_include_type type,
    const char* requesting_source,
    size_t include_depth) override;

  void ReleaseInclude(shaderc_include_result* data) override;

  std::set<std::filesystem::path> available_include_files_;
  std::vector<std::unique_ptr<shaderc_include_result>> includes_in_use_;
};

} // namespace plex::graphics

#endif // PLEX_GRAPHICS_VULKAN_VULKAN_SHADER_INCLUDE_HANDLER_H
