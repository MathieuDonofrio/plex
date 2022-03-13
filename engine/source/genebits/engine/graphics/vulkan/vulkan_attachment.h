#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_ATTACHMENT_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_ATTACHMENT_H

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{

class VulkanAttachment
{
public:

  struct LayoutTransitionSequence
  {
    VkImageLayout initial_layout;
    VkImageLayout layout;
    VkImageLayout final_layout;
  };

  VulkanAttachment(uint32_t attachment_index,
    VkFormat format,
    VkAttachmentLoadOp load_op,
    VkAttachmentStoreOp store_op,
    LayoutTransitionSequence layout_transition_sequence)
    : attachment_description_({}), attachment_reference_({})
  {

    attachment_description_.format = format;
    attachment_description_.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment_description_.loadOp = load_op;
    attachment_description_.storeOp = store_op;
    // Stencil not used for now
    attachment_description_.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment_description_.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // Defines the initial layout (Let's vulkan know if there is a need to convert to another layout when calling the
    // subpass for the first time
    attachment_description_.initialLayout = initial_layout;
    // Defines what layout is needed at the after this subpass
    attachment_description_.finalLayout = final_layout;

    attachment_reference_.attachment = attachment_index;
    // Defines the layout that is needed during the subpass
    attachment_reference_.layout = layout;
  }

  [[nodiscard]] const VkAttachmentReference& GetAttachmentReference() const
  {
    return attachment_reference_;
  }

protected:
  VkAttachmentDescription attachment_description_;
  VkAttachmentReference attachment_reference_;
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_ATTACHMENT_H
