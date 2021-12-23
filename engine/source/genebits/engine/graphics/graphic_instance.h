#ifndef GENEBITS_ENGINE_GRAPHICS_GRAPHIC_INSTANCE_H
#define GENEBITS_ENGINE_GRAPHICS_GRAPHIC_INSTANCE_H

#include <genebits/engine/graphics/graphics_debug_level.h>
#include <genebits/engine/graphics/window.h>
namespace genebits::engine
{

class GraphicInstance
{
public:
  GraphicInstance(const GraphicInstance&) = delete;
  GraphicInstance() = delete;
  GraphicInstance& operator=(const GraphicInstance&) = delete;

  virtual ~GraphicInstance() = default;

  GraphicInstance(const char* application_name, bool is_debug, GraphicsDebugLevel debug_message_severity_threshold);

protected:
  bool is_debug_;

  const char* application_name_;

private:
  GraphicsDebugLevel debug_message_severity_threshold_;
};

} // namespace genebits::engine
#endif
