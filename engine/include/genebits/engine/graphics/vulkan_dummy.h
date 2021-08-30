#ifndef GENEBITS_VULKAN_DUMMY_H
#define GENEBITS_VULKAN_DUMMY_H

#include "window.h"

namespace genebits::engine
{

class VulkanDummy
{
public:
  ///
  /// Enum flag used to indicate at which level of severity should the debug messages be printed
  ///
  enum class DebugMessageSeverityThreshold : uint32_t
  {
    Trace = BitFlag(0),
    Info = BitFlag(1),
    Warn = BitFlag(2),
    Error = BitFlag(3)
  };

  VulkanDummy(Window* window_ptr,
    const char* name,
    bool use_messenger,
    DebugMessageSeverityThreshold debug_message_severity_threshold);
  void DummyFunction(VulkanDummy) {}; // Use to remove errors from unused variables

private:
  class VulkanInstanceT;
  VulkanInstanceT* instance_ptr = nullptr;
};

} // namespace genebits::engine
#endif // GENEBITS_VULKAN_DUMMY_H
