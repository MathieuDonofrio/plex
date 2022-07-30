#include "vulkan_instance.h"

#include "plex/config/version.h"
#include "plex/debug/logging.h"
#include "plex/graphics/vulkan/vulkan_config.h"

#include <format>
#include <iostream>

namespace plex
{
VulkanInstance::VulkanInstance(
  std::shared_ptr<Window> window, const std::string& application_name, GraphicsDebugLevel debug_level)
  : application_name_(application_name), debug_level_(debug_level), debug_messenger_(nullptr)
{

  if (!Initialize(dynamic_cast<VulkanCapableWindow*>(window.get())))
  {
    LOG_ERROR("Failed to initialize vulkan instance");

    return;
  }

  LOG_INFO("Vulkan instance initialized");
}

VulkanInstance::~VulkanInstance()
{
#ifndef NDEBUG
  DestroyDebugUtilsMessengerEXT(instance_, debug_messenger_, nullptr);
  debug_messenger_ = nullptr;
#endif

  vkDestroyInstance(instance_, nullptr);
  instance_ = VK_NULL_HANDLE;

  LOG_INFO("Vulkan instance destroyed");
}

bool VulkanInstance::Initialize(VulkanCapableWindow* window)
{
#ifndef NDEBUG
  if (!CheckValidationLayerSupport()) return false;
#endif

  VkApplicationInfo app_info {};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = application_name_.c_str();
  app_info.applicationVersion = VK_MAKE_VERSION(PLEX_VERSION_MAJOR, PLEX_VERSION_MINOR, PLEX_VERSION_PATCH);
  app_info.pEngineName = "Plex";
  app_info.engineVersion = VK_MAKE_VERSION(PLEX_VERSION_MAJOR, PLEX_VERSION_MINOR, PLEX_VERSION_PATCH);
  app_info.apiVersion = VK_API_VERSION_1_3;

  std::vector<const char*> extensions = window->GetRequiredInstanceExtensions();

#ifndef NDEBUG
  extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

  VkInstanceCreateInfo create_info {};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;
  create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  create_info.ppEnabledExtensionNames = extensions.data();

#ifndef NDEBUG
  create_info.enabledLayerCount = static_cast<uint32_t>(cVulkanValidationLayers.size());
  create_info.ppEnabledLayerNames = cVulkanValidationLayers.data();

  // The debug messenger we set up after the instance creation does not handle messages about creation and destruction
  // of the instance. To work around this we need to create a separate debug messenger for those two cases.
  // For this debug messenger we can use verbose messaging since it won't impact performance.
  VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
  debug_create_info = {};
  debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  debug_create_info.messageSeverity =
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                  | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                  | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  debug_create_info.pfnUserCallback = DebugMessengerCallback;
  debug_create_info.pUserData = nullptr;

  VkValidationFeaturesEXT validation_features {};
  validation_features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
  validation_features.enabledValidationFeatureCount = static_cast<uint32_t>(cVulkanValidationFeatures.size());
  validation_features.pEnabledValidationFeatures = cVulkanValidationFeatures.data();
  validation_features.pNext = &debug_create_info;

  create_info.pNext = &validation_features;
#else
  create_info.enabledLayerCount = 0;
  create_info.pNext = nullptr;
#endif

  if (vkCreateInstance(&create_info, nullptr, &instance_) != VK_SUCCESS)
  {
    LOG_ERROR("Failed to create vulkan instance");

    return false;
  }

#ifndef NDEBUG
  if (!SetupDebugMessenger())
  {
    LOG_ERROR("Failed to setup debug messenger");

    return false;
  }
#endif

  return instance_ != VK_NULL_HANDLE;
}

bool VulkanInstance::CheckValidationLayerSupport()
{
  for (const auto& layer_name : cVulkanValidationLayers)
  {
    if (!IsValidationLayerSupported(layer_name))
    {
      LOG_ERROR("Validation layer not supported: {}", layer_name);

      return false;
    }
  }

  return true;
}

bool VulkanInstance::IsValidationLayerSupported(const std::string& layer_name)
{
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  for (const auto& layerProperties : available_layers)
  {
    if (layer_name == layerProperties.layerName) return true;
  }

  return false;
}

bool VulkanInstance::SetupDebugMessenger()
{
  VkDebugUtilsMessengerCreateInfoEXT create_info {};

  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

  create_info.messageSeverity = 0;

  switch (debug_level_)
  {
  case GraphicsDebugLevel::Trace: create_info.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
  case GraphicsDebugLevel::Info: create_info.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
  case GraphicsDebugLevel::Warn: create_info.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
  case GraphicsDebugLevel::Error: create_info.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT; break;
  }

  create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                            | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

  create_info.pfnUserCallback = DebugMessengerCallback;
  create_info.pUserData = nullptr;

  return CreateDebugUtilsMessengerEXT(instance_, &create_info, nullptr, &debug_messenger_) == VK_SUCCESS;
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::DebugMessengerCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
  VkDebugUtilsMessageTypeFlagsEXT,
  const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
  void*)
{
  switch (message_severity)
  {
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
  {
    LOG_TRACE("[Vulkan Messenger] {}", callback_data->pMessage);
    break;
  }
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
  {
    LOG_INFO("[Vulkan Messenger] {}", callback_data->pMessage);
    break;
  }
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
  {
    LOG_WARN("[Vulkan Messenger] {}", callback_data->pMessage);
    break;
  }
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
  {
    LOG_ERROR("[Vulkan Messenger] {}", callback_data->pMessage);
    break;
  }
  default:
  {
    LOG_ERROR("[Vulkan Messenger] **Unknown Severity!** {}", callback_data->pMessage);
    break;
  }
  }

  return VK_FALSE; // returning VK_TRUE is for testing the validation layers themselves usually
}

VkResult VulkanInstance::CreateDebugUtilsMessengerEXT(VkInstance handle,
  const VkDebugUtilsMessengerCreateInfoEXT* create_info,
  const VkAllocationCallbacks* allocator,
  VkDebugUtilsMessengerEXT* debug_messenger_callback)
{
  auto create_debug_messenger_function = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
    vkGetInstanceProcAddr(handle, "vkCreateDebugUtilsMessengerEXT"));

  if (create_debug_messenger_function != nullptr)
  {
    return create_debug_messenger_function(handle, create_info, allocator, debug_messenger_callback);
  }
  else
  {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void VulkanInstance::DestroyDebugUtilsMessengerEXT(
  VkInstance handle, VkDebugUtilsMessengerEXT debug_messenger, const VkAllocationCallbacks* allocator_ptr)
{
  auto destroy_debug_messenger_function = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
    vkGetInstanceProcAddr(handle, "vkDestroyDebugUtilsMessengerEXT"));

  if (destroy_debug_messenger_function != nullptr)
  {
    destroy_debug_messenger_function(handle, debug_messenger, allocator_ptr);
  }
}

} // namespace plex
