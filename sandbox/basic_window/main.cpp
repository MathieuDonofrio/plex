
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "plex/debug/logging.h"
#include "plex/events/listener.h"
#include "plex/graphics/renderer.h"
#include "plex/graphics/window.h"

using namespace plex;
using namespace plex::graphics;

struct TestWindowListener : public Listener<TestWindowListener,
                              WindowCloseEvent,
                              WindowFocusEvent,
                              WindowMaximizeEvent,
                              WindowIconifyEvent,
                              WindowResizeEvent,
                              WindowKeyboardEvent,
                              WindowCursorEnterEvent,
                              WindowCursorMoveEvent,
                              WindowMouseButtonEvent,
                              WindowMouseScrollEvent,
                              WindowFramebufferResizeEvent>
{
  TestWindowListener(EventBus& bus) : Listener(bus) {}

  void Listen(const WindowCloseEvent&)
  {
    std::cout << "window close event" << std::endl;
  }

  void Listen(const WindowFocusEvent& event)
  {
    std::cout << "window focus event: " << ((event.state == WindowFocusEvent::FocusState::Gained) ? "Gained" : "Lost")
              << std::endl;
  }

  void Listen(const WindowMaximizeEvent& event)
  {
    std::cout << "window maximise event: " << event.maximized << std::endl;
  }

  void Listen(const WindowIconifyEvent& event)
  {
    std::cout << "window iconify event: " << event.iconified << std::endl;
  }

  void Listen(const WindowResizeEvent& event)
  {
    std::cout << "window resize event: " << event.width << ", " << event.height << std::endl;
  }

  void Listen(const WindowKeyboardEvent& event)
  {
    std::cout << "keyboard event: " << KeyCodeToString(event.keycode) << ", " << static_cast<uint32_t>(event.modifiers)
              << ", " << static_cast<uint32_t>(event.action) << std::endl;

    if (event.keycode == plex::W && event.action == plex::ButtonEvent::Action::Pressed)
    {
      std::cout << "Window monitor query: " << event.window->GetMonitorWidth() << "x"
                << event.window->GetMonitorHeight() << std::endl;
    }
  }

  void Listen(const WindowCursorMoveEvent& event)
  {
    std::cout << "Cursor move event: x=" << event.pos_x << " y=" << event.pos_y << std::endl;
  }

  void Listen(const WindowCursorEnterEvent& event)
  {
    std::cout << "Cursor enter/leave event: " << static_cast<uint32_t>(event.cursor_hover_state) << std::endl;
  }

  void Listen(const WindowMouseButtonEvent& event)
  {
    std::cout << "Mouse button event: " << static_cast<uint32_t>(event.action) << ", "
              << static_cast<uint32_t>(event.button) << ", Mods: " << static_cast<uint32_t>(event.modifiers)
              << std::endl;
  }

  void Listen(const WindowMouseScrollEvent& event)
  {
    std::cout << "Mouse scroll event: offset: " << event.vertical_offset << std::endl;
  }

  void Listen(const WindowFramebufferResizeEvent& event)
  {
    std::cout << "Window framebuffer resize event: " << event.width << "x" << event.height << std::endl;
  }
};

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

  TestWindowListener listener { bus };

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

  auto vertex_shader_code = LoadShaderCodeFromFile("../../sandbox/basic_window/assets/shader.vert.spv");
  auto fragment_shader_code = LoadShaderCodeFromFile("../../sandbox/basic_window/assets/shader.frag.spv");

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
