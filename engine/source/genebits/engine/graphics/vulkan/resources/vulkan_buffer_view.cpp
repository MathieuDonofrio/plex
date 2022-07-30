#include "vulkan_buffer_view.h"

#include "genebits/engine/debug/logging.h"

#include <utility>

namespace genebits::engine
{

VulkanBufferView::VulkanBufferView(std::shared_ptr<VulkanDevice> device, VkFormat format, VkBuffer buffer_handle)
  : device_(std::move(device))
{
  VkBufferViewCreateInfo buffer_view_create_info = {};
  buffer_view_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
  buffer_view_create_info.buffer = buffer_handle;
  buffer_view_create_info.format = format;
  buffer_view_create_info.offset = 0;
  buffer_view_create_info.range = VK_WHOLE_SIZE;

  if (vkCreateBufferView(device_->GetHandle(), &buffer_view_create_info, nullptr, &buffer_view_handle_) != VK_SUCCESS)
  {
    LOG_ERROR("Failed to create buffer view");
  }
}
VulkanBufferView::~VulkanBufferView() {
  if (buffer_view_handle_ != VK_NULL_HANDLE)
  {
    vkDestroyBufferView(device_->GetHandle(), buffer_view_handle_, nullptr);
  }
}

} // namespace genebits::engine
