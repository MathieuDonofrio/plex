#ifndef GENEBITS_VULKAN_FRAME_BUFFER_GROUP_H
#define GENEBITS_VULKAN_FRAME_BUFFER_GROUP_H

#include <memory>
#include <vector>

#include "genebits/engine/graphics/vulkan/vulkan_frame_buffer.h"

#include "vulkan/vulkan_core.h"

namespace genebits::engine
{

class VulkanFrameBufferGroup
{
public:


private:
  std::vector<std::shared_ptr<VulkanFrameBuffer>> frame_buffers_;
};

} // namespace genebits::engine

#endif // GENEBITS_VULKAN_FRAME_BUFFER_GROUP_H
