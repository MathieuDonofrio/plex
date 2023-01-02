#include "vulkan_shader_include_handler.h"

#include <iostream>

#include "plex/debug/logging.h"

namespace plex::graphics
{

namespace fs = std::filesystem;

shaderc_include_result* VulkanShaderIncludeHandler::GetInclude(
  const char* requested_source, shaderc_include_type type, const char* requesting_source, size_t include_depth)
{
  std::cout << "Requested source: " << requested_source << std::endl;
  std::cout << "Requesting source: " << requesting_source << std::endl;
  std::cout << "Include depth: " << include_depth << std::endl;
  std::cout << "Type: " << (type == shaderc_include_type::shaderc_include_type_relative ? "relative" : "standard")
            << std::endl;

  //  fs::path requested_path;
  //  if (type == shaderc_include_type_relative)
  //  {
  //    requested_path = fs::path(requesting_source).parent_path() / requested_source;
  //  }
  //  else
  //  {
  //    requested_path = requested_source;
  //  }

  auto include_result = std::make_unique<shaderc_include_result>();
  include_result->source_name = nullptr;
  include_result->source_name_length = 0;
  include_result->content = nullptr;
  include_result->content_length = 0;
  include_result->user_data = nullptr;

  includes_in_use_.emplace_back(std::move(include_result));
  return includes_in_use_.back().get();
}

void VulkanShaderIncludeHandler::ReleaseInclude(shaderc_include_result* data) {
  std::cout << "Releasing include: " << data->source_name << std::endl;
}

bool VulkanShaderIncludeHandler::AddIncludeDirectory(
  const std::filesystem::path& path, bool recursive, uint32_t max_depth)
{
  if (!fs::exists(path))
  {
    LOG_ERROR("Include path does not exist: {}", path.string());
    return false;
  }

  for (const auto& file : fs::directory_iterator(path))
  {
    if (recursive && max_depth > 0 && file.is_directory())
    {
      AddIncludeDirectory(file.path(), true, max_depth - 1);
    }
    else
    {
      AddIncludeFile(file.path());
    }
  }
  return true;
}
} // namespace plex::graphics