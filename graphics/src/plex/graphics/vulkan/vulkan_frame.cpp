#include "plex/graphics/vulkan/vulkan_frame.h"

namespace plex::graphics
{

VulkanFrame::VulkanFrame(const VulkanFrameCreateInfo& create_info)
  : image_(create_info.image,
    create_info.device->GetHandle(),
    create_info.width,
    create_info.height,
    create_info.format,
    create_info.usage),
    image_available_semaphore_(create_info.device->GetHandle()),
    image_acquired_semaphore_(create_info.device->GetHandle()),
    render_complete_semaphore_(create_info.device->GetHandle()),
    image_available_fence_(create_info.device->GetHandle()), render_complete_fence_(create_info.device->GetHandle()),
    command_pool_(create_info.device->GetHandle(), create_info.queue_family_index),
    main_command_buffer_(command_pool_.AllocateNative())
{}

} // namespace plex::graphics