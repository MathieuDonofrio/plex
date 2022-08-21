#include "vulkan_loader.h"

#include "plex/debug/logging.h"
#include "vulkan_function_table.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <memory>
#include <string>
#include <vector>

namespace plex::graphics::vkapi::loader
{

namespace
{

  static std::shared_ptr<VkInstance_T> instance_; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
  static std::shared_ptr<VkDevice_T> device_; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
  static std::vector<const char*> extensions_; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

  class LibraryWrapper
  {
  private:
    HMODULE library_ { nullptr };

  public:
    LibraryWrapper(const LibraryWrapper&) = delete;
    LibraryWrapper& operator=(const LibraryWrapper&) = delete;
    LibraryWrapper(LibraryWrapper&&) = delete;
    LibraryWrapper& operator=(LibraryWrapper&&) = delete;

    LibraryWrapper(const char* library_name) noexcept : library_ { LoadLibraryA(library_name) }
    {
      if (!library_)
      {
        LOG_ERROR("Failed to load vulkan library: ");
      }
    }

    ~LibraryWrapper() noexcept
    {
      if (library_)
      {
        FreeLibrary(library_);
      }
    }

    template<typename T>
    T GetProcAddress(const char* function_name) const noexcept
    {
      if (!library_) return nullptr;
      return reinterpret_cast<T>(::GetProcAddress(library_, function_name));
    }
  };

  const LibraryWrapper _vulkan_library { "vulkan-1.dll" };

  void SetInstance(VkInstance instance)
  {
    instance_ = std::shared_ptr<VkInstance_T>(instance,
      [](VkInstance instance)
      {
        if (instance) GetFunctionTable().vkDestroyInstance(instance, nullptr);
      });
  }

  void SetDevice(VkDevice device)
  {
    device_ = std::shared_ptr<VkDevice_T>(device,
      [](VkDevice device)
      {
        if (device) GetFunctionTable().vkDestroyDevice(device, nullptr);
      });
  }

  void SetExtensions(const std::vector<const char*>& extensions)
  {
    extensions_ = extensions;
  };

  const std::vector<const char*>& GetExtensions() noexcept
  {
    return extensions_;
  };

  using VoidFunctionPtr = void (*)();

  VkResult LoadFunctionGroup(void* handle, VoidFunctionPtr(resolver)(void*, const char*), const char* group_name)
  {
    const auto [start_index, count] = GetFunctionGroupLoadInfo(group_name);

    auto function_table = reinterpret_cast<void (**)()>(&GetFunctionTable());

    for (uint32_t offset = 0; offset != count; ++offset)
    {
      const auto index = start_index + offset;

      const auto function_name = GetFunctionName(index);
      if (!function_name)
      {
        LOG_ERROR("Failed to get name of vulkan function: " + std::string(group_name));
        return VK_ERROR_INITIALIZATION_FAILED;
      }

      const auto function_pointer = resolver(handle, function_name);
      if (!function_pointer)
      {
        LOG_ERROR("Failed to load vulkan function: " + std::string(function_name));
        return VK_ERROR_INITIALIZATION_FAILED;
      }

      function_table[index] = function_pointer;
    }

    return VK_SUCCESS;
  }

  VulkanResult LoadFunctionTableWithInstance()
  {
    if (!instance_)
    {
      LOG_ERROR("No vulkan instance available, call CreateInstance first");
      return VK_ERROR_INITIALIZATION_FAILED;
    }

    const auto LibVkGetInstanceProcAddr =
      _vulkan_library.GetProcAddress<VoidFunctionPtr (*)(void*, const char*)>("vkGetInstanceProcAddr");
    if (!LibVkGetInstanceProcAddr)
    {
      LOG_ERROR("Failed to load vulkan function: vkGetInstanceProcAddr from vulkan library");
      return VK_ERROR_INITIALIZATION_FAILED;
    }

    const auto get_instance_result = LoadFunctionGroup(nullptr, LibVkGetInstanceProcAddr, "vkGetInstanceProcAddr");
    if (get_instance_result != VK_SUCCESS) return get_instance_result;

    const auto vkGetInstanceProcAddr =
      reinterpret_cast<VoidFunctionPtr (*)(void*, const char*)>(GetFunctionTable().vkGetInstanceProcAddr);

    const auto global_result = LoadFunctionGroup(nullptr, vkGetInstanceProcAddr, "global");
    if (global_result != VK_SUCCESS) return global_result;

    std::vector<const char*> function_groups = extensions_;
    function_groups.emplace_back("core");

    for (const auto& function_group : function_groups)
    {
      const auto result = LoadFunctionGroup(instance_.get(), vkGetInstanceProcAddr, function_group);
      if (result != VK_SUCCESS) return result;
    }

    return VK_SUCCESS;
  }

} // namespace

VkInstance GetInstance() noexcept
{
  return instance_.get();
}

VkDevice GetDevice() noexcept
{
  return device_.get();
}

VulkanResult CreateInstance(const VkApplicationInfo& app_info,
  const std::vector<const char*>& extensions,
  const std::vector<const char*>& layers,
  const void* create_info_extension)
{
  VkInstanceCreateInfo create_info {};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;
  create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  create_info.ppEnabledExtensionNames = extensions.data();
  create_info.enabledLayerCount = static_cast<uint32_t>(layers.size());
  create_info.ppEnabledLayerNames = layers.data();
  create_info.pNext = create_info_extension;

  const auto vkCreateInstance = _vulkan_library.GetProcAddress<PFN_vkCreateInstance>("vkCreateInstance");
  if (!vkCreateInstance)
  {
    LOG_ERROR("Failed to load vulkan function: vkCreateInstance");
    return VK_ERROR_INITIALIZATION_FAILED;
  }

  VkInstance instance {};
  VulkanResult result = vkCreateInstance(&create_info, nullptr, &instance);

  if (result)
  {
    SetInstance(instance);
    SetExtensions(extensions);
    result = LoadFunctionTableWithInstance();
  }

  return result;
}

VulkanResult UseDevice(VkDevice device)
{
  if (!instance_)
  {
    LOG_ERROR("No vulkan instance available, call CreateInstance first");
    return VK_ERROR_INITIALIZATION_FAILED;
  }

  const auto vkGetDeviceProcAddr =
    reinterpret_cast<VoidFunctionPtr (*)(void*, const char*)>(GetFunctionTable().vkGetDeviceProcAddr);
  if (!vkGetDeviceProcAddr)
  {
    LOG_ERROR("VkGetDeviceProcAddr is not available, call LoadFunctionTableWithInstance first");
    return VK_ERROR_INITIALIZATION_FAILED;
  }

  auto function_groups = GetExtensions();
  function_groups.emplace(function_groups.begin(), "core");

  for (const auto& function_group : function_groups)
  {
    const auto result = LoadFunctionGroup(device, vkGetDeviceProcAddr, function_group);
    if (result != VK_SUCCESS) return result;
  }

  return VK_SUCCESS;
}

} // namespace plex::graphics::vkapi::loader
