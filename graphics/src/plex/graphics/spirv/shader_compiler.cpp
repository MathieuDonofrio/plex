#include "plex/graphics/spirv/shader_compiler.h"

#include <fstream>

#include "plex/debug/logging.h"

namespace plex::graphics
{

namespace fs = std::filesystem;

namespace
{
  shaderc_shader_kind FromShaderType(ShaderType type)
  {
    switch (type)
    {
    case ShaderType::Vertex: return shaderc_glsl_vertex_shader;
    case ShaderType::Fragment: return shaderc_glsl_fragment_shader;
    case ShaderType::Compute: return shaderc_glsl_compute_shader;
    default:
    {
      LOG_WARN("Unknown shader stage flag: {}", static_cast<int>(type));
      return shaderc_glsl_infer_from_source;
    }
    }
  }

  void SpirvToolsConsumeMessage([[maybe_unused]] spv_message_level_t level,
    [[maybe_unused]] const char* source,
    [[maybe_unused]] const spv_position_t& position,
    [[maybe_unused]] const char* message)
  {
    switch (level)
    {
    case SPV_MSG_FATAL:
    case SPV_MSG_INTERNAL_ERROR:
    case SPV_MSG_ERROR:
      LOG_ERROR("SPIR-V error: {}, line: {}, column: {}, message: {}", source, position.line, position.column, message);
      break;
    case SPV_MSG_WARNING:
      LOG_WARN(
        "SPIR-V warning: {}, line: {}, column: {}, message: {}", source, position.line, position.column, message);
      break;
    case SPV_MSG_INFO:
      LOG_INFO("SPIR-V info: {}, line: {}, column: {}, message: {}", source, position.line, position.column, message);
      break;
    case SPV_MSG_DEBUG:
      LOG_TRACE("SPIR-V debug: {}, line: {}, column: {}, message: {}", source, position.line, position.column, message);
      break;
    default: LOG_WARN("Unknown SPIR-V message level: {}", static_cast<int>(level)); break;
    }
  }

} // namespace

ShaderCompiler::ShaderCompiler() : spirv_tools_(SPV_ENV_VULKAN_1_3)
{
#ifndef NDEBUG // TODO: Add a different flag for this and synchronize with vulkan extensions
  options_.SetGenerateDebugInfo();
#endif

  options_.SetSourceLanguage(shaderc_source_language_glsl);
  options_.SetTargetSpirv(shaderc_spirv_version_1_6);
  options_.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);

  spirv_tools_.SetMessageConsumer(SpirvToolsConsumeMessage);

  if (!compiler_.IsValid())
  {
    error_message_ = "Failed to initialize shader compiler";
    valid_ = false;
  }

  if (!spirv_tools_.IsValid())
  {
    if (!error_message_.empty()) error_message_ += ", ";
    error_message_ = "Failed to initialize SPIR-V tools";
    valid_ = false;
  }
}

std::optional<ShaderData> ShaderCompiler::Compile(
  const std::string& source, const std::filesystem::path& path, ShaderType type)
{
  if (!valid_)
  {
    LOG_ERROR("Failed to compile shader: {}", error_message_);
    return std::nullopt;
  }

  auto compile_result = CompileToSpv(source, fs::absolute(path).string(), type);
  if (!compile_result) return std::nullopt;

  auto compiled_spv = std::vector<uint32_t>(compile_result->cbegin(), compile_result->cend());

  if (validation_enabled_ && !spirv_tools_.Validate(compiled_spv))
  {
    error_message_ = "SPIR-V validation failed";
    return std::nullopt;
  }

  return ShaderData { std::move(compiled_spv), type };
}

std::optional<shaderc::CompilationResult<uint32_t>> ShaderCompiler::CompileToSpv(
  const std::string& source, const std::string& absolute_path, ShaderType type)
{
  const auto shader_kind = FromShaderType(type);

  auto compile_result =
    compiler_.CompileGlslToSpv(source.data(), source.size(), shader_kind, absolute_path.c_str(), options_);
  if (compile_result.GetCompilationStatus() != shaderc_compilation_status_success)
  {
    error_message_ = compile_result.GetErrorMessage();
    return std::nullopt;
  }

  return compile_result;
}

} // namespace plex::graphics
