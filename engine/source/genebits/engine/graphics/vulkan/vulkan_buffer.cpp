#include "vulkan_buffer.h"

#include "genebits/engine/debug/logging.h"

#include <utility>

namespace genebits::engine
{

VulkanBuffer::VulkanBuffer(VulkanBufferCreateInfo& create_info)
  : VulkanResource(create_info.binding_frequency, create_info.descriptor_type, create_info.shader_stage_flags),
    device_(std::move(create_info.device))
{
  VkBufferCreateInfo buf_info = {};
  buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buf_info.size = create_info.size;
  buf_info.usage = create_info.usage;
  buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  VmaAllocationCreateInfo alloc_info = {};
  alloc_info.usage = VMA_MEMORY_USAGE_AUTO;
  alloc_info.flags = create_info.memory_properties;

  const auto allocator = device_->GetAllocator();
  if (vmaCreateBuffer(allocator, &buf_info, &alloc_info, &buffer_handle_, &allocation_, nullptr) != VK_SUCCESS)
  {
    LOG_ERROR("Failed to create Vulkan buffer.");
    return;
  }

  if (vkCreateBuffer(device_->GetHandle(), &buf_info, nullptr, &buffer_handle_) != VK_SUCCESS)
  {
    LOG_ERROR("Failed to create vulkan buffer");
    return;
  }
}

VulkanBuffer::~VulkanBuffer()
{
  if (allocation_)
  {
    vmaDestroyBuffer(device_->GetAllocator(), buffer_handle_, allocation_);
    allocation_ = VK_NULL_HANDLE;
    buffer_handle_ = VK_NULL_HANDLE;
  }
}

VkBuffer VulkanBuffer::GetBuffer() const noexcept
{
  return buffer_handle_;
}

} // namespace genebits::engine