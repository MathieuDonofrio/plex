#include "plex/graphics/spirv/shader_compiler.h"

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

  shaderc_optimization_level FromOptimizationLevel(OptimizationLevel optimization_level)
  {
    {
      switch (optimization_level)
      {
      case OptimizationLevel::None: return shaderc_optimization_level_zero;
      case OptimizationLevel::Size: return shaderc_optimization_level_size;
      case OptimizationLevel::Speed: return shaderc_optimization_level_performance;
      default:
      {
        LOG_WARN("Unknown optimization level: {}", static_cast<int>(optimization_level));
        return shaderc_optimization_level_zero;
      }
      }
    }
  }

  shaderc_spirv_version FromSpirvVersion(SpirvVersion version)
  {
    switch (version)
    {
    case SpirvVersion::Spirv_1_0: return shaderc_spirv_version_1_0;
    case SpirvVersion::Spirv_1_1: return shaderc_spirv_version_1_1;
    case SpirvVersion::Spirv_1_2: return shaderc_spirv_version_1_2;
    case SpirvVersion::Spirv_1_3: return shaderc_spirv_version_1_3;
    case SpirvVersion::Spirv_1_4: return shaderc_spirv_version_1_4;
    case SpirvVersion::Spirv_1_5: return shaderc_spirv_version_1_5;
    case SpirvVersion::Spirv_1_6: return shaderc_spirv_version_1_6;
    default:
    {
      LOG_WARN("Unknown spirv version: {}", static_cast<int>(version));
      return shaderc_spirv_version_1_0;
    }
    }
  }

  shaderc_target_env FromTargetEnvironment(TargetEnvironment env)
  {
    switch (env)
    {
    case TargetEnvironment::Vulkan: return shaderc_target_env_vulkan;
    case TargetEnvironment::OpenGL: return shaderc_target_env_opengl;
    default:
    {
      LOG_WARN("Unknown target environment: {}", static_cast<int>(env));
      return shaderc_target_env_vulkan;
    }
    }
  }

  spv_target_env FromSpirvTargetEnvironment(SpirvVersion version, TargetEnvironment env)
  {
    switch (env)
    {
    case TargetEnvironment::Vulkan:
    {
      switch (version)
      {
      case SpirvVersion::Spirv_1_0: return SPV_ENV_VULKAN_1_0;
      case SpirvVersion::Spirv_1_1:
      case SpirvVersion::Spirv_1_2:
      case SpirvVersion::Spirv_1_3: return SPV_ENV_VULKAN_1_1;
      case SpirvVersion::Spirv_1_4: return SPV_ENV_VULKAN_1_1_SPIRV_1_4;
      case SpirvVersion::Spirv_1_5: return SPV_ENV_VULKAN_1_2;
      case SpirvVersion::Spirv_1_6: return SPV_ENV_VULKAN_1_3;
      default:
      {
        LOG_WARN("Unknown spirv version: {}", static_cast<int>(version));
        return SPV_ENV_VULKAN_1_0;
      }
      }
    }
    case TargetEnvironment::OpenGL:
    {
      switch (version)
      {
      case SpirvVersion::Spirv_1_0: return SPV_ENV_UNIVERSAL_1_0;
      case SpirvVersion::Spirv_1_1: return SPV_ENV_UNIVERSAL_1_1;
      case SpirvVersion::Spirv_1_2: return SPV_ENV_UNIVERSAL_1_2;
      case SpirvVersion::Spirv_1_3: return SPV_ENV_UNIVERSAL_1_3;
      case SpirvVersion::Spirv_1_4: return SPV_ENV_UNIVERSAL_1_4;
      case SpirvVersion::Spirv_1_5: return SPV_ENV_UNIVERSAL_1_5;
      case SpirvVersion::Spirv_1_6: return SPV_ENV_UNIVERSAL_1_6;
      default:
      {
        LOG_WARN("Unknown spirv version: {}", static_cast<int>(version));
        return SPV_ENV_UNIVERSAL_1_0;
      }
      }
    }
    default:
    {
      LOG_WARN("Unknown target environment: {}", static_cast<int>(env));
      return SPV_ENV_UNIVERSAL_1_0;
    }
    }
  }

  shaderc_source_language FromSourceLanguage(SourceLanguage lang)
  {
    switch (lang)
    {
    case SourceLanguage::GLSL: return shaderc_source_language_glsl;
    case SourceLanguage::HLSL: return shaderc_source_language_hlsl;
    default:
    {
      LOG_WARN("Unknown source language: {}", static_cast<int>(lang));
      return shaderc_source_language_glsl;
    }
    }
  }

  shaderc_env_version FromTargetEnvironmentVersion(TargetEnvironmentVersion version)
  {
    switch (version)
    {
    case TargetEnvironmentVersion::Vulkan_1_0: return shaderc_env_version_vulkan_1_0;
    case TargetEnvironmentVersion::Vulkan_1_1: return shaderc_env_version_vulkan_1_1;
    case TargetEnvironmentVersion::Vulkan_1_2: return shaderc_env_version_vulkan_1_2;
    case TargetEnvironmentVersion::Vulkan_1_3: return shaderc_env_version_vulkan_1_3;
    case TargetEnvironmentVersion::OpenGL_4_5: return shaderc_env_version_opengl_4_5;
    default:
    {
      LOG_WARN("Unknown target environment version: {}", static_cast<int>(version));
      return shaderc_env_version_vulkan_1_0;
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

ShaderCompiler::ShaderCompiler(const ShaderCompilerOptions& compiler_options)
  : spirv_tools_(FromSpirvTargetEnvironment(compiler_options.spirv_version, compiler_options.target_environment))
{
  if (compiler_options.generate_debug_information) options_.SetGenerateDebugInfo();
  options_.SetTargetSpirv(FromSpirvVersion(compiler_options.spirv_version));
  options_.SetTargetEnvironment(FromTargetEnvironment(compiler_options.target_environment),
    FromTargetEnvironmentVersion(compiler_options.target_environment_version));

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

std::optional<ShaderData> ShaderCompiler::Compile(const std::string& source,
  const std::filesystem::path& path,
  ShaderType type,
  ShaderCompilationOptions compile_options)
{
  if (!valid_)
  {
    LOG_ERROR("Failed to compile shader: {}", error_message_);
    return std::nullopt;
  }

  options_.SetOptimizationLevel(FromOptimizationLevel(compile_options.optimization_level));
  options_.SetSourceLanguage(FromSourceLanguage(compile_options.source_language));

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
