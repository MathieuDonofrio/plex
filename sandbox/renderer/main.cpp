
#include <chrono>
#include <filesystem>
#include <fstream>

#include "plex/debug/logging.h"
#include "plex/events/listener.h"
#include "plex/graphics/renderer.h"
#include "plex/graphics/window.h"

using namespace plex;
using namespace plex::graphics;

struct FPSCounter
{
  std::chrono::high_resolution_clock::time_point last_time_ = std::chrono::high_resolution_clock::now();

  double GetFPS()
  {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - last_time_).count();
    last_time_ = now;
    return 1000000.0 / duration;
  }
};

std::vector<char> LoadShaderCodeFromFile(const std::string& filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) [[unlikely]]
  {
    std::filesystem::path path(filename);
    std::filesystem::path absolute_path = std::filesystem::absolute(path);

    LOG_ERROR("Failed to open shader file: {}", absolute_path.string());
    return {};
  }

  std::streamsize fileSize = file.tellg();
  std::vector<char> buffer(fileSize);

  file.seekg(0);
  file.read(buffer.data(), fileSize);

  file.close();

  return buffer;
}

std::unique_ptr<Material> material;

void RecordCommandBuffer(CommandBuffer* primary_buffer)
{
  primary_buffer->Begin();
  primary_buffer->BeginRenderPass();

  primary_buffer->FirstTriangleTest(material.get());

  primary_buffer->EndRenderPass();
  primary_buffer->End();
}

int main(int, char**)
{
  // Create Window

  EventBus bus;

  constexpr WindowCreationHints hints = WindowCreationHints::Defaults;

  std::unique_ptr<Window> window = CreateWindow("Hello world", 512, 512, &bus, hints);

  // Create Renderer

  graphics::RendererCreateInfo renderer_info;
  renderer_info.window_handle = window.get();
  renderer_info.application_name = "Basic Window";
  renderer_info.debug_level = DebugLevel::Info;
  renderer_info.present_mode = PresentMode::FIFO;
  renderer_info.buffering_mode = BufferingMode::Double;

  std::unique_ptr<Renderer> renderer = CreateRenderer(renderer_info, BackendType::Vulkan);

  // Create material

  auto vertex_shader_code = LoadShaderCodeFromFile("../../sandbox/renderer/assets/shader.vert.spv");
  auto fragment_shader_code = LoadShaderCodeFromFile("../../sandbox/renderer/assets/shader.frag.spv");

  auto vertex_shader = renderer->CreateShader(vertex_shader_code.data(), vertex_shader_code.size(), ShaderType::Vertex);
  auto fragment_shader =
    renderer->CreateShader(fragment_shader_code.data(), fragment_shader_code.size(), ShaderType::Fragment);

  MaterialCreateInfo material_create_info {};
  material_create_info.vertex_shader = vertex_shader.get();
  material_create_info.fragment_shader = fragment_shader.get();

  material = renderer->CreateMaterial(material_create_info);

  // Loop

  FPSCounter fps_counter;

  while (!window->IsClosing())
  {
    window->PollEvents();

    CommandBuffer* primary_buffer = renderer->AquireNextFrame();

    RecordCommandBuffer(primary_buffer);

    renderer->Render();

    renderer->Present();

    window->SetTitle("FPS: " + std::to_string(fps_counter.GetFPS()));

    std::this_thread::yield();
  }

  return 0;
}
