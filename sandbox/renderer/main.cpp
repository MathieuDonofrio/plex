
#include <filesystem>
#include <fstream>

#include "plex/debug/logging.h"
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

std::unique_ptr<Window> window;
std::unique_ptr<Renderer> renderer;

std::unique_ptr<Material> material;

Buffer<Vertex> vertex_buffer;
Buffer<uint16_t> index_buffer;

void RecordCommandBuffer(CommandBuffer* primary_buffer)
{
  primary_buffer->Begin();
  primary_buffer->BeginRenderPass();

  primary_buffer->BindVertexBuffer(vertex_buffer);
  primary_buffer->BindIndexBuffer(index_buffer);

  primary_buffer->BindMaterial(material.get());

  primary_buffer->DrawIndexed(index_buffer.size());

  primary_buffer->EndRenderPass();
  primary_buffer->End();
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

  renderer = CreateRenderer(renderer_info, BackendType::Vulkan);

  // Create material

  LOG_INFO("Compiling shaders...");

  auto vertex_shader = renderer->CreateShader("../../sandbox/renderer/assets/shader.vert", ShaderType::Vertex);
  auto fragment_shader = renderer->CreateShader("../../sandbox/renderer/assets/shader.frag", ShaderType::Fragment);

  LOG_INFO("Shaders compiled");

  MaterialCreateInfo material_create_info {};
  material_create_info.vertex_shader = vertex_shader.get();
  material_create_info.fragment_shader = fragment_shader.get();

  material = renderer->CreateMaterial(material_create_info);

  // Create Buffers

  vertex_buffer = renderer->CreateBuffer<Vertex>(3, BufferUsageFlags::Vertex, MemoryPropertyFlags::HostVisible);

  Vertex* data = vertex_buffer.Map();

  data[0].pos = { 0.0f, -0.5f };
  data[0].color = { 1.0f, 0.0f, 0.0f };
  data[1].pos = { 0.5, 0.5 };
  data[1].color = { 0.0f, 1.0f, 0.0f };
  data[2].pos = { -0.5, 0.5 };
  data[2].color = { 0.0f, 0.0f, 1.0f };

  vertex_buffer.Unmap();

  index_buffer = renderer->CreateBuffer<uint16_t>(3, BufferUsageFlags::Index, MemoryPropertyFlags::HostVisible);

  uint16_t* index_data = index_buffer.Map();

  index_data[0] = 0;
  index_data[1] = 1;
  index_data[2] = 2;

  index_buffer.Unmap();

  // Loop

  FPSCounter fps_counter;

  while (!window->IsClosing())
  {
    window->PollEvents();

    CommandBuffer* primary_buffer = renderer->AcquireNextFrame();

    if (primary_buffer == nullptr) continue;

    RecordCommandBuffer(primary_buffer);

    renderer->Render();

    renderer->Present();

    window->SetTitle("FPS: " + std::to_string(fps_counter.GetFPS()));

    std::this_thread::yield();
  }

  renderer->WaitIdle();

  // TODO destroy materials & shaders

  return 0;
}
