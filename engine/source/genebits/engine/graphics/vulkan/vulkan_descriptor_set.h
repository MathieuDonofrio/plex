#ifndef GENEBITS_ENGINE_GRAPHICS_VULKANDESCRIPTORSET_H
#define GENEBITS_ENGINE_GRAPHICS_VULKANDESCRIPTORSET_H

#include "vulkan/vulkan_core.h"

namespace genebits::engine {

class VulkanDescriptorSet {
public:
private:

  VkDescriptorSetLayout descriptor_set_layout_;
  VkDescriptorSet descriptor_set_;

};

}

#endif // GENEBITS_ENGINE_GRAPHICS_VULKANDESCRIPTORSET_H