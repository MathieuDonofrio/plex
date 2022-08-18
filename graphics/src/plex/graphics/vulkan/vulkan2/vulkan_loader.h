#ifndef GENEBITS_VULKAN_LOADER_H
#define GENEBITS_VULKAN_LOADER_H

#include "vulkan_function_table.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan_core.h>

#include <vector>

namespace plex::vkapi::loader
{

///
/// Initializes the global vulkan instance
///
/// @param app_info The application info to use when creating the instance
/// @param extensions The extensions to enable when creating the instance
/// @param layers The layers to enable when creating the instance
/// @param create_info_extension pointer to use for the pNext member of the create info struct
/// @return VK_SUCCESS if the instance was successfully created
VkResult CreateInstance(const VkApplicationInfo& app_info,
  const std::vector<const char*>& extensions,
  const std::vector<const char*>& layers,
  const void* create_info_extension = nullptr);

///
/// Loads the function table using the global instance created by CreateInstance
///
/// @note This function must be called after a successful call to CreateInstance
/// @return VK_SUCCESS if the function table was successfully loaded
VkResult LoadFunctionTableWithInstance(const std::vector<const char*>& extensions);

///
/// Loads the function table using the given instance
///
/// @param instance The instance to use to load the function table
/// @return VK_SUCCESS if the function table was successfully loaded
VkResult LoadFunctionTableWithDevice(VkDevice device);

} // namespace plex::vkapi::loader

#endif // GENEBITS_VULKAN_LOADER_H
