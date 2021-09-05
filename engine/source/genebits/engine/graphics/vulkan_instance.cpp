#include "genebits/engine/graphics/vulkan_instance.h"

#include "genebits/engine/config/version.h"
#include "genebits/engine/debug/logging.h"

#include <format>

namespace genebits::engine
{

VulkanInstance::VulkanInstance(VulkanCapableWindow* vulkan_capable_window,
  const char* application_name,
  bool use_debug_messenger,
  DebugMessageSeverityThreshold debug_message_severity_threshold)
  : debug_message_severity_threshold_(debug_message_severity_threshold)
{
  VkApplicationInfo app_info {};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = application_name;
  app_info.applicationVersion = VK_MAKE_VERSION(GENEBITS_VERSION_MAJOR, GENEBITS_VERSION_MINOR, GENEBITS_VERSION_PATCH);
  app_info.pEngineName = "Genebits graphic engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo create_info {};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;

  VkDebugUtilsMessengerCreateInfoEXT debug_create_info {};
  if (use_debug_messenger)
  {
    if (QueryValidationLayersSupport(validation_layer_names_))
    {
      create_info.enabledLayerCount = static_cast<uint32_t>(validation_layer_names_.size());
      create_info.ppEnabledLayerNames = validation_layer_names_.data();

      PopulateDebugMessengerCreateInfo(debug_create_info);
      create_info.pNext = &debug_create_info;
    }
  }
  else
  {
    create_info.enabledLayerCount = 0;
    create_info.pNext = nullptr;
  }

  std::vector<const char*> required_extensions = vulkan_capable_window->GetRequiredInstanceExtensions();

  if (use_debug_messenger) { required_extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); }

  create_info.enabledExtensionCount = static_cast<uint32_t>(required_extensions.size());
  create_info.ppEnabledExtensionNames = required_extensions.data();

  if (vkCreateInstance(&create_info, nullptr, &instance_handle_) != VK_SUCCESS)
  {
    LOG_ERROR("Failed to create Vulkan instance");
  }

  if (use_debug_messenger) { CreateDebugMessenger(instance_handle_, &debug_messenger_); }
}

VulkanInstance::~VulkanInstance()
{
  if (debug_messenger_ != nullptr) { DestroyDebugUtilsMessengerEXT(instance_handle_, debug_messenger_, nullptr); }

  vkDestroyInstance(instance_handle_, nullptr);
}

void VulkanInstance::CreateDebugMessenger(VkInstance instance_handle, VkDebugUtilsMessengerEXT* debug_messenger_ptr)
{
  VkDebugUtilsMessengerCreateInfoEXT create_info;
  PopulateDebugMessengerCreateInfo(create_info);

  if (CreateDebugUtilsMessengerEXT(instance_handle, &create_info, nullptr, debug_messenger_ptr))
  {
    LOG_ERROR("Failed to set up the vulkan's debug messenger");
  }
}

bool VulkanInstance::QueryValidationLayersSupport(const std::vector<const char*>& validation_layer_names)
{
  uint32_t available_layer_count;
  vkEnumerateInstanceLayerProperties(&available_layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(available_layer_count);
  vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers.data());

  uint32_t found_layer_count = 0;

  for (const char* layer_name : validation_layer_names)
  {
    for (const VkLayerProperties& layer_properties : available_layers)
    {
      if (std::string(layer_name).compare(layer_properties.layerName) == 0)
      {
        found_layer_count++;
        break;
      }
    }
  }

  return validation_layer_names.size() == found_layer_count;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::DebugMessengerCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
  VkDebugUtilsMessageTypeFlagsEXT message_type,
  const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
  void* user_data_ptr)
{
  uint32_t severity_threshold = *static_cast<uint32_t*>(user_data_ptr);
  if (message_severity < severity_threshold) { return VK_FALSE; }

  constexpr const char* message_types[] { "General", "Validation", "Performance" };
  std::string message_types_str;

  for (uint32_t i = 0; i < 3; ++i)
  {
    if (message_type & (1 << i))
    {
      if (!message_types_str.empty()) { message_types_str.append(", "); }
      message_types_str.append(message_types[i]);
    }
  }

  switch (message_severity)
  {
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    LOG_TRACE("Vulkan validation layer\n\tSeverity: Verbose\n\tType: {}\n\tMessage: {}",
      message_types_str,
      callback_data_ptr->pMessage);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    LOG_INFO("Vulkan validation layer\n\tSeverity: Info\n\tType: {}\n\tMessage: {}",
      message_types_str,
      callback_data_ptr->pMessage);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    LOG_WARN("Vulkan validation layer\n\tSeverity: Warning\n\tType: {}\n\tMessage: {}",
      message_types_str,
      callback_data_ptr->pMessage);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    LOG_ERROR("Vulkan validation layer\n\tSeverity: Error\n\tType: {}\n\tMessage: {}",
      message_types_str,
      callback_data_ptr->pMessage);
    break;
  default:
    LOG_ERROR("Vulkan validation layer\n\tSeverity: *Unknown severity*\n\tType: {}\n\tMessage: {}",
      message_types_str,
      callback_data_ptr->pMessage);
    break;
  }

  return VK_FALSE; // returning VK_TRUE is for testing the validation layers themselves usually
}

VkResult VulkanInstance::CreateDebugUtilsMessengerEXT(VkInstance handle,
  const VkDebugUtilsMessengerCreateInfoEXT* create_info_ptr,
  const VkAllocationCallbacks* allocator_ptr,
  VkDebugUtilsMessengerEXT* debug_messenger_callback_ptr)
{
  auto create_debug_messenger_function_ptr = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
    vkGetInstanceProcAddr(handle, "vkCreateDebugUtilsMessengerEXT"));

  if (create_debug_messenger_function_ptr != nullptr)
  {
    return create_debug_messenger_function_ptr(handle, create_info_ptr, allocator_ptr, debug_messenger_callback_ptr);
  }
  else
  {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void VulkanInstance::DestroyDebugUtilsMessengerEXT(
  VkInstance handle, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks* allocator_ptr)
{
  auto destroy_debug_messenger_function_ptr = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
    vkGetInstanceProcAddr(handle, "vkDestroyDebugUtilsMessengerEXT"));

  if (destroy_debug_messenger_function_ptr != nullptr)
  {
    destroy_debug_messenger_function_ptr(handle, debug_messenger, allocator_ptr);
  }
}

void VulkanInstance::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info) noexcept
{
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
                                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info.pfnUserCallback = DebugMessengerCallback;
  create_info.pUserData = &debug_message_severity_threshold_;
}

} // namespace genebits::engine
