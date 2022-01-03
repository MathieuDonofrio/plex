#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_DEVICE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_DEVICE_H

#include "genebits/engine/debug/logging.h"
#include "genebits/engine/graphics/device.h"
#include "vulkan_adapter_queries.h"
#include "vulkan_queue_family_indices.h"

#include <memory>
#include <set>
#include <vector>

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanDevice : Device
{
public:
  VulkanDevice(std::shared_ptr<VkPhysicalDevice_T> adapter,
    VkSurfaceKHR surface_handle,
    const std::vector<std::string>& required_extensions)
    : adapter_(std::move(adapter))
  {
    if (!Initialize(surface_handle, required_extensions)) { LOG_ERROR("Failed to initialize vulkan device"); }
    else
    {
      LOG_INFO("Vulkan device initialized");
    }
  }

  VulkanDevice(const VulkanDevice&) = delete;
  VulkanDevice& operator=(const VulkanDevice&) = delete;
  VulkanDevice(VulkanDevice&&) = delete;
  VulkanDevice& operator=(VulkanDevice&&) = delete;

  ~VulkanDevice() override
  {
    vkDestroyDevice(device_handle_, nullptr);
    device_handle_ = VK_NULL_HANDLE;
    LOG_INFO("Vulkan device destroyed");
  }

  [[nodiscard]] const VulkanQueueFamilyIndices& GetQueueFamilyIndices() const noexcept
  {
    return queue_family_indices_;
  }

  [[nodiscard]] const VkDevice GetHandle() const noexcept
  {
    return device_handle_;
  }

private:
  VkDevice device_handle_;
  std::shared_ptr<VkPhysicalDevice_T> adapter_;

  VulkanQueueFamilyIndices queue_family_indices_;
  VkQueue graphics_queue_;
  VkQueue present_queue_;

  bool Initialize(VkSurfaceKHR surface_handle, const std::vector<std::string>& required_extensions)
  {
    queue_family_indices_ = VulkanAdapterQueries::GetAdapterQueueFamilyIndices(adapter_.get(), surface_handle);

    const float queue_priority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos =
      GetQueueCreateInfos(queue_family_indices_, &queue_priority);

    std::vector<const char*> required_extensions_c_str = ToCharPointerVector(required_extensions);

    VkPhysicalDeviceFeatures device_features {};
    device_features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo create_info =
      GetDeviceCreateInfo(required_extensions_c_str, queue_create_infos, &device_features);

    if (vkCreateDevice(adapter_.get(), &create_info, nullptr, &device_handle_) != VK_SUCCESS)
    {
      LOG_ERROR("Failed to create logical device");
      return false;
    }

    vkGetDeviceQueue(device_handle_, queue_family_indices_.GetGraphicsFamilyIndex(), 0, &graphics_queue_);
    vkGetDeviceQueue(device_handle_, queue_family_indices_.GetPresentFamilyIndex(), 0, &present_queue_);

    return true;
  }

  [[nodiscard]] std::vector<const char*> ToCharPointerVector(const std::vector<std::string>& required_extensions) const
  {
    std::vector<const char*> required_extensions_c_str;
    for (const auto& extension : required_extensions)
    {
      required_extensions_c_str.emplace_back(extension.c_str());
    }
    return required_extensions_c_str;
  }

  [[nodiscard]] std::vector<VkDeviceQueueCreateInfo> GetQueueCreateInfos(
    const VulkanQueueFamilyIndices& family_indices, const float* queue_priority) const
  {
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<uint32_t> unique_queue_families = { family_indices.GetGraphicsFamilyIndex(),
      family_indices.GetPresentFamilyIndex() };

    for (uint32_t queue_family : unique_queue_families)
    {
      VkDeviceQueueCreateInfo queue_create_info {};
      queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queue_create_info.queueFamilyIndex = queue_family;
      queue_create_info.queueCount = 1;
      queue_create_info.pQueuePriorities = queue_priority;
      queue_create_infos.push_back(queue_create_info);
    }

    return queue_create_infos;
  }

  VkDeviceCreateInfo GetDeviceCreateInfo(const std::vector<const char*>& required_extensions,
    std::vector<VkDeviceQueueCreateInfo>& queue_create_infos,
    VkPhysicalDeviceFeatures* device_features) const
  {
    VkDeviceCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    create_info.pQueueCreateInfos = queue_create_infos.data();

    create_info.pEnabledFeatures = device_features;

    create_info.enabledExtensionCount = static_cast<uint32_t>(required_extensions.size());
    create_info.ppEnabledExtensionNames = required_extensions.data();

    create_info.enabledLayerCount = 0;
    return create_info;
  }
};

} // namespace genebits::engine
#endif
