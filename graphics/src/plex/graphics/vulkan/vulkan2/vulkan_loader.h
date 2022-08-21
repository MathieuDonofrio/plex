#ifndef GENEBITS_VULKAN_LOADER_H
#define GENEBITS_VULKAN_LOADER_H

#include "vulkan_api_helpers.h"
#include "vulkan_function_table.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan_core.h>

#include <vector>

namespace plex::graphics::vkapi::loader
{

///
/// Get the global instance
/// @return the global instance
VkInstance GetInstance() noexcept;

///
/// Get the global device
/// @return the global device
VkDevice GetDevice() noexcept;

///
/// Initializes the global vulkan instance
///
/// @param app_info The application info to use when creating the instance
/// @param extensions The extensions to enable when creating the instance
/// @param layers The layers to enable when creating the instance
/// @param create_info_extension pointer to use for the pNext member of the create info struct
/// @return The result of the instance creation if failed, otherwise the result of the function table loading
VulkanResult CreateInstance(const VkApplicationInfo& app_info,
  const std::vector<const char*>& extensions,
  const std::vector<const char*>& layers,
  const void* create_info_extension);

///
/// Reloads the core and extensions function table using the given device
/// Certain core and extension function pointers can be fetched to point directly to the device functions and reduce
/// overhead
///
/// @param device The instance to use to reload the function table with
/// @return The result of the function table reloading
/// @warning This function takes ownership of the device handle and will destroy it when finished
VulkanResult UseDevice(VkDevice device);

} // namespace plex::graphics::vkapi::loader

#endif // GENEBITS_VULKAN_LOADER_H
