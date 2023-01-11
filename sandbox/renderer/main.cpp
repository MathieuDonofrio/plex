
#include <filesystem>
#include <fstream>

#include "plex/debug/logging.h"
#include "plex/graphics/renderer.h"
#include "plex/graphics/window.h"
#include "plex/graphics/color.h"

using namespace plex;
using namespace plex::graphics;

class FPSCounter
{
public:
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;

  FPSCounter() : last_time_(std::chrono::high_resolution_clock::now()) {}

  bool Update()
  {
    using namespace std::chrono_literals;
    using namespace std::chrono;

    frame_count_++;

    auto now = Clock::now();
    auto elapsed = now - last_time_;

    if (elapsed >= 1s)
    {
      fps_ = frame_count_ * (1.0 / duration_cast<duration<double>>(elapsed).count());
      frame_count_ = 0;
      last_time_ = now;
      return true;
    }

    return false;
  }

  [[nodiscard]] double GetFPS() const noexcept
  {
    return fps_;
  }

private:
  TimePoint last_time_;

  double frame_count_ = 0;
  double fps_ = 0;
};

class DeltaTime
{
public:
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;

  DeltaTime() : last_time_(Clock::now()) {}

  void Update()
  {
    using namespace std::chrono;

    auto now = Clock::now();
    auto elapsed = now - last_time_;
    last_time_ = now;

    delta_time_ = duration_cast<duration<double>>(elapsed).count();
  }

  [[nodiscard]] float GetDeltaTime() const noexcept
  {
    return static_cast<float>(delta_time_);
  }

private:
  TimePoint last_time_;
  double delta_time_ = 0;
};

std::string LoadShaderCodeFromFile(const std::string& filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) [[unlikely]]
  {
    std::filesystem::path path(filename);
    std::filesystem::path absolute_path = std::filesystem::absolute(path);

    LOG_ERROR("Failed to open shader file: {}", absolute_path.string());
    return {};
  }

  std::streamsize file_size = file.tellg();
  std::string source;
  source.resize(file_size);

  file.seekg(0);
  file.read(source.data(), file_size);

  file.close();

  return source;
}

std::unique_ptr<Window> window;
std::unique_ptr<Renderer> renderer;

std::unique_ptr<Material> material;

Buffer<Vertex> vertex_buffer;
Buffer<uint16_t> index_buffer;

float wheel = 0;
float wheel_speed = 0.2f;

void OnInit()
{
  // Create material

  LOG_INFO("Compiling shaders...");

  ShaderCompileOptions options {};
  options.language = ShaderLanguage::GLSL;

#ifdef NDEBUG
  options.optimization = ShaderOptimization::Speed;
#else
  options.optimization = ShaderOptimization::None;
#endif

  auto vertex_shader_location = "../../sandbox/renderer/assets/shader.vert";
  auto fragment_shader_location = "../../sandbox/renderer/assets/shader.frag";

  auto vertex_shader = renderer->CreateShader(
    LoadShaderCodeFromFile(vertex_shader_location), vertex_shader_location, ShaderType::Vertex, options);
  auto fragment_shader = renderer->CreateShader(
    LoadShaderCodeFromFile(fragment_shader_location), fragment_shader_location, ShaderType::Fragment, options);

  LOG_INFO("Shaders compiled");

  MaterialCreateInfo material_create_info {};
  material_create_info.vertex_shader = vertex_shader.get();
  material_create_info.fragment_shader = fragment_shader.get();

  material = renderer->CreateMaterial(material_create_info);

  // Make CPU accessible vertex buffer

  vertex_buffer = renderer->CreateBuffer<Vertex>(3, BufferUsageFlags::Vertex, MemoryUsage::CPU_To_GPU);

  Vertex* data = vertex_buffer.Map();

  data[0].pos = { 0.0f, -0.5f };
  data[0].color = { 1.0f, 0.0f, 0.0f };
  data[1].pos = { 0.5, 0.5 };
  data[1].color = { 0.0f, 1.0f, 0.0f };
  data[2].pos = { -0.5, 0.5 };
  data[2].color = { 0.0f, 0.0f, 1.0f };

  vertex_buffer.Unmap();

  // Make GPU-Only accessible index buffer

  Buffer<uint16_t> staging_index_buffer =
    renderer->CreateBuffer<uint16_t>(3, BufferUsageFlags::TransferSource, MemoryUsage::CPU_To_GPU);

  uint16_t* index_data = staging_index_buffer.Map();

  index_data[0] = 0;
  index_data[1] = 1;
  index_data[2] = 2;

  staging_index_buffer.Unmap();

  index_buffer = renderer->CreateBuffer<uint16_t>(
    3, BufferUsageFlags::Index | BufferUsageFlags::TransferDestination, MemoryUsage::GPU_Only);

  renderer->SubmitImmediate(
    [&](CommandBuffer* command_buffer)
    {
      command_buffer->Begin();
      command_buffer->CopyBuffer(staging_index_buffer, index_buffer);
      command_buffer->End();
    });
}

void OnRecord(CommandBuffer* command_buffer, float delta)
{
  command_buffer->Begin();
  command_buffer->BeginRenderPass();

  wheel = fmodf(wheel + 360 * wheel_speed * delta, 360);

  Vertex* data = vertex_buffer.Map();

  auto color = RGBfloat::FromHSV(static_cast<float>(wheel), 1, 1);

  data[0].color = color;
  data[1].color = color;
  data[2].color = color;

  vertex_buffer.Unmap();

  command_buffer->BindVertexBuffer(vertex_buffer);
  command_buffer->BindIndexBuffer(index_buffer);

  command_buffer->BindMaterial(material.get());

  command_buffer->DrawIndexed(index_buffer.size());

  command_buffer->EndRenderPass();
  command_buffer->End();
}

int main(int, char**)
{
  // Create Window

  constexpr WindowCreationHints hints = WindowCreationHints::Defaults;

  window = CreateWindow("Hello world", 512, 512, hints);

  // Create Renderer

  graphics::RendererCreateInfo renderer_info;
  renderer_info.window = window.get();
  renderer_info.application_name = "Basic Window";
  renderer_info.debug_level = DebugLevel::Info;
  renderer_info.present_mode = PresentMode::Immediate;
  renderer_info.buffering_mode = BufferingMode::Double;
  renderer_info.shader_debug_info_enabled = true;
  renderer_info.shader_validation_enabled = true;

  renderer = CreateRenderer(renderer_info, BackendType::Vulkan);

  // Init

  OnInit();

  // Loop

  FPSCounter fps_counter;
  DeltaTime delta_time;

  while (!window->IsClosing())
  {
    delta_time.Update();

    window->PollEvents();

    CommandBuffer* primary_buffer = renderer->AcquireNextFrame();

    if (primary_buffer == nullptr) continue;

    OnRecord(primary_buffer, delta_time.GetDeltaTime());

    renderer->Render();
    renderer->Present();

    if (fps_counter.Update())
    {
      window->SetTitle("FPS: " + std::to_string(static_cast<int>(std::round(fps_counter.GetFPS()))));
    }

    std::this_thread::yield();
  }

  renderer->WaitIdle();

  // TODO destroy materials & shaders

  return 0;
}
