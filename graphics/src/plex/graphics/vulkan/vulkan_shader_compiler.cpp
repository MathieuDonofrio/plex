#include "vulkan_shader_compiler.h"

#include <fstream>

#include "plex/debug/logging.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"

namespace plex::graphics
{

namespace fs = std::filesystem;

namespace
{

  shaderc_shader_kind FromShaderStageFlags(ShaderStageFlags stage)
  {
    switch (stage)
    {
    case ShaderStageFlags::Vertex: return shaderc_glsl_vertex_shader;
    case ShaderStageFlags::Fragment: return shaderc_glsl_fragment_shader;
    case ShaderStageFlags::Compute: return shaderc_glsl_compute_shader;
    case ShaderStageFlags::All: return shaderc_glsl_infer_from_source;
    default:
    {
      LOG_WARN("Unknown shader stage flag: {}", static_cast<int>(stage));
      return shaderc_glsl_infer_from_source;
    }
    }
  }

  std::optional<std::vector<char>> ReadFile(const fs::path& path)
  {
    std::ifstream file(path, std::ios::ate | std::ios::binary);

    if (!file.is_open()) [[unlikely]]
      return std::nullopt;

    std::streamsize fileSize = file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
  }

} // namespace

VulkanShaderCompiler::VulkanShaderCompiler() : spirv_tools_(SPV_ENV_VULKAN_1_3)
{
#ifndef NDEBUG // TODO: Add a different flag for this and synchronize with vulkan extensions
  options_.SetGenerateDebugInfo();
#endif

  options_.SetSourceLanguage(shaderc_source_language_glsl);
  options_.SetTargetSpirv(shaderc_spirv_version_1_6);
}

std::optional<VulkanSpvBinary> VulkanShaderCompiler::Compile(const fs::path& path, ShaderStageFlags stage)
{
  if (!compiler_.IsValid())
  {
    error_message_ = "Failed to initialize shader compiler";
    return std::nullopt;
  }

  if (!spirv_tools_.IsValid())
  {
    error_message_ = "Failed to initialize SPIR-V tools";
    return std::nullopt;
  }

  if (!fs::exists(path))
  {
    error_message_ = "Shader file does not exist: " + fs::absolute(path).string();
    return std::nullopt;
  }

  const auto absolute_path = fs::absolute(path).string();
  const auto shader_kind = FromShaderStageFlags(stage);
  shaderc::PreprocessedSourceCompilationResult preprocess_result;
  {
    auto buffer = ReadFile(path);
    if (!buffer)
    {
      error_message_ = "Failed to read shader file: " + fs::absolute(path).string();
      return std::nullopt;
    }

    preprocess_result =
      compiler_.PreprocessGlsl(buffer->data(), buffer->size(), shader_kind, absolute_path.c_str(), options_);
    if (preprocess_result.GetCompilationStatus() != shaderc_compilation_status_success)
    {
      error_message_ = preprocess_result.GetErrorMessage();
      return std::nullopt;
    }
  }

  size_t preprocessed_size = std::distance(preprocess_result.cbegin(), preprocess_result.cend());
  auto compile_result = compiler_.CompileGlslToSpv(
    preprocess_result.cbegin(), preprocessed_size, shader_kind, absolute_path.c_str(), options_);

  if (compile_result.GetCompilationStatus() != shaderc_compilation_status_success)
  {
    error_message_ = compile_result.GetErrorMessage();
    return std::nullopt;
  }

  const size_t compiled_size = std::distance(compile_result.cbegin(), compile_result.cend());
  // const size_t padding = compiled_size % sizeof(uint32_t);
  std::vector<uint32_t> spv(compiled_size);
  std::memcpy(spv.data(), compile_result.cbegin(), compiled_size);

  spirv_tools_.SetMessageConsumer(
    [](spv_message_level_t level, const char* source, const spv_position_t& position, const char* message)
    {
      switch (level)
      {
      case SPV_MSG_FATAL:
      case SPV_MSG_INTERNAL_ERROR:
      case SPV_MSG_ERROR:
        LOG_ERROR(
          "SPIR-V error: {}, line: {}, column: {}, message: {}", source, position.line, position.column, message);
        break;
      case SPV_MSG_WARNING:
        LOG_WARN(
          "SPIR-V warning: {}, line: {}, column: {}, message: {}", source, position.line, position.column, message);
        break;
      case SPV_MSG_INFO:
        LOG_INFO("SPIR-V info: {}, line: {}, column: {}, message: {}", source, position.line, position.column, message);
        break;
      case SPV_MSG_DEBUG:
        LOG_TRACE(
          "SPIR-V debug: {}, line: {}, column: {}, message: {}", source, position.line, position.column, message);
        break;
      default: LOG_WARN("Unknown SPIR-V message level: {}", static_cast<int>(level)); break;
      }
    });

  if (!spirv_tools_.Validate(spv))
  {
    error_message_ = "SPIR-V validation failed";
    return std::nullopt;
  }

  return VulkanSpvBinary(std::move(spv), absolute_path);
}

// TODO: split into recursive and non-recursive versions
bool VulkanShaderCompiler::AddIncludeDirectory(const fs::path& path, bool recursive, uint32_t max_depth)
{
  return include_handler_->AddIncludeDirectory(path, recursive, max_depth);
}

}; // namespace plex::graphics
