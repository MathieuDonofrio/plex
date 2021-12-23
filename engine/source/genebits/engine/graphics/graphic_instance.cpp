#include "graphic_instance.h"
#include <iostream>

namespace genebits::engine
{

GraphicInstance::GraphicInstance(
  const char* applicationName, bool isDebug, GraphicsDebugLevel debugMessageSeverityThreshold)
  : application_name_(applicationName), is_debug_(isDebug),
    debug_message_severity_threshold_(debugMessageSeverityThreshold)
{}

// GraphicInstance::~GraphicInstance()
//{
//   std::cout << "base" << std::endl;
// }

} // namespace genebits::engine
