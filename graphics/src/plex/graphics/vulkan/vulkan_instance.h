#ifndef PLEX_GRAPHICS_VULKAN_VULKAN_INSTANCE_H
#define PLEX_GRAPHICS_VULKAN_VULKAN_INSTANCE_H

#include <vector>

#include "plex/graphics/renderer.h"
#include "plex/graphics/vulkan/api/vulkan_api.h"

namespace plex::graphics
{
class VulkanInstance
{
public:
  VulkanInstance(const std::string& application_name, DebugLevel debug_level, std::vector<const char*> extensions);

  ~VulkanInstance();

  [[nodiscard]] DebugLevel GetDebugLevel() const
  {
    return debug_level_;
  }

  [[nodiscard]] const std::string& GetApplicationName() const
  {
    return application_name_;
  }

  [[nodiscard]] VkInstance GetHandle() const noexcept
  {
    return instance_;
  }

private:
  ///
  /// Callback used by vulkan to provide feedback on the application's use of vulkan if an event of interest occurs
  ///
  /// @param[in] message_severity The severity of the event that triggered the callback
  /// @param[in] message_type Bit flags telling which type of event(s) triggered the callback
  /// @param[in] callback_data_ptr Pointer to the callback's data, including the message
  ///
  /// @return bool used for debugging the validation layers themselves, usually false when not testing them
  ///
  static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    [[maybe_unused]] const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
    void*);

  ///
  /// Creates the debug messenger.
  ///
  /// Proxy function used to delegate the call to the real function loaded at runtime.
  ///
  /// @param instance Vulkan instance.
  /// @param create_info Create info struct.
  /// @param allocator Pointer to allocation callbacks.
  /// @param debug_messenger_callback Reference to function called when a debug message is sent.
  ///
  /// @return Whether or not the creation failed.
  ///
  static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* create_info,
    const VkAllocationCallbacks* allocator,
    VkDebugUtilsMessengerEXT* debug_messenger_callback);

  ///
  /// Destroys the debug messenger.
  ///
  /// Proxy function used to delegate the call to the real function loaded at runtime.
  ///
  /// @param instance Vulkan instance
  /// @param debug_messenger Debug messenger to destroy.
  /// @param allocator Pointer to allocation callbacks.
  ///
  static void DestroyDebugUtilsMessengerEXT(
    VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks* allocator_ptr);

private:
  VkInstance instance_;

  std::string application_name_;

  VkDebugUtilsMessengerEXT debug_messenger_;
  DebugLevel debug_level_;
};
} // namespace plex::graphics

#endif