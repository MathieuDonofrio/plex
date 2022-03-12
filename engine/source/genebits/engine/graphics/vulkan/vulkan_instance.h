#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_INSTANCE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_INSTANCE_H

#include "genebits/engine/graphics/graphics_debug_level.h"
#include "genebits/engine/utilities/enumerator.h"
#include "vulkan_capable_window.h"

#include <vulkan/vulkan_core.h>

#include <string>

namespace genebits::engine
{

class VulkanInstance
{
public:
  ///
  /// Parametric constructor used to create a vulkan instance
  /// TODO update documentation
  /// @param[in] vulkan_capable_window Class implementing the VulkanCapableWindow interface for querying the required
  /// extensions for the multimedia library in use.
  /// @param[in] use_debug_messenger Whether the debug messenger should be set up and used
  ///
  VulkanInstance(VulkanCapableWindow* window_handle,
    const char* application_name,
    bool use_debug_messenger,
    GraphicsDebugLevel debug_level);

  VulkanInstance(const VulkanInstance&) = delete;
  VulkanInstance& operator=(const VulkanInstance&) = delete;
  VulkanInstance(VulkanInstance&&) = delete;
  VulkanInstance& operator=(VulkanInstance&&) = delete;

  ~VulkanInstance();

  [[nodiscard]] const VkInstance GetHandle() const noexcept;

  ///
  ///
  /// @warning Requires debug level INFO to print
  ///
  const void PrintAvailableExtensions();

private:
  VkInstance instance_;

  VkDebugUtilsMessengerEXT debug_messenger_ = nullptr;
  bool use_debug_messenger_;

  const char* application_name_;

  GraphicsDebugLevel debug_message_severity_threshold_;

  const std::vector<const char*> validation_layer_names_ = { "VK_LAYER_KHRONOS_validation" };

  bool Initialize(VulkanCapableWindow* window_handle);

  void CreateDebugMessenger(VkInstance instance_handle, VkDebugUtilsMessengerEXT* debug_messenger_ptr);

  bool QueryValidationLayersSupport(const std::vector<const char*>& validation_layer_names);

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
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
    void*);

  // Proxy function since the real function needs to be acquired at runtime
  VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* create_info_ptr,
    const VkAllocationCallbacks* allocator_ptr,
    VkDebugUtilsMessengerEXT* debug_messenger_callback_ptr);

  // Proxy function since the real function needs to be acquired at runtime
  void DestroyDebugUtilsMessengerEXT(
    VkInstance instance_handle, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks* allocator_ptr);
  void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info) noexcept;
};

} // namespace genebits::engine
#endif