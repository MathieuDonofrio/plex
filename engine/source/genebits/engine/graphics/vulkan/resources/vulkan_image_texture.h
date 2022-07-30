#ifndef GENEBITS_ENGINE_GRAPHICS_VULKAN_IMAGE_H
#define GENEBITS_ENGINE_GRAPHICS_VULKAN_IMAGE_H

#include <vulkan/vulkan_core.h>

namespace genebits::engine
{


/*
Sampled Image: A descriptor type that represents an image view,
 and supports filtered (sampled) and unfiltered read-only acccess in a shader.

Sampler: An object that contains state that controls how sampled image data is sampled (or filtered) when accessed in a shader.
 Also a descriptor type describing the object. Represented by a VkSampler object.

Combined Image Sampler: A descriptor type that includes both a sampled image and a sampler.

Storage Image: A descriptor type that represents an image view, and supports unfiltered loads, stores, and atomics in a shader.

Input Attachment: A descriptor type that represents an image view, and supports unfiltered read-only access in a shader, only at the fragment’s location in the view.

Uniform Buffer: A descriptor type that represents a buffer, and supports read-only access in a shader.

Dynamic Uniform Buffer: A uniform buffer whose offset is specified each time the uniform buffer is bound to a command buffer via a descriptor set.

Uniform Texel Buffer: A descriptor type that represents a buffer view, and supports unfiltered, formatted, read-only access in a shader.

Storage Buffer: A descriptor type that represents a buffer, and supports reads, writes, and atomics in a shader.

Dynamic Storage Buffer: A storage buffer whose offset is specified each time the storage buffer is bound to a command buffer via a descriptor set.

Storage Texel Buffer: A descriptor type that represents a buffer view, and supports unfiltered, formatted reads, writes, and atomics in a shader.
*/

/*
 A descriptor type that represents an image view, and supports filtered (sampled) and unfiltered read-only access in a
 shader
 */
class VulkanImage
{
public:
private:
};

} // namespace genebits::engine

#endif // GENEBITS_ENGINE_GRAPHICS_VULKAN_SAMPLED_IMAGE_H
