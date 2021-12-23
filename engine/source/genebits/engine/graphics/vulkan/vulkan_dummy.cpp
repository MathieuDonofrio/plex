#include "vulkan_instance.h"
#include <genebits/engine/graphics/vulkan/vulkan_instance.h>
#include <genebits/engine/graphics/vulkan_dummy.h>

namespace genebits::engine
{
using VulkanInstanceT = VulkanInstance;

VulkanDummy::VulkanDummy(Window* window_ptr,
  const char* name,
  bool use_messenger,
  DebugMessageSeverityThreshold debug_message_severity_threshold)
{
  //  instance_ptr =
  //    reinterpret_cast<VulkanDummy::VulkanInstanceT*>(new
  //    VulkanInstance(dynamic_cast<VulkanCapableWindow*>(window_ptr),
  //      name,
  //      use_messenger,
  //      static_cast<VulkanInstance::DebugMessageSeverityThreshold>(debug_message_severity_threshold)));
}

} // namespace genebits::engine