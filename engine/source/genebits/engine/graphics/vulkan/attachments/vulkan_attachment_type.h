#ifndef GENEBITS_VULKAN_ATTACHMENT_TYPE_H
#define GENEBITS_VULKAN_ATTACHMENT_TYPE_H

#include "genebits/engine/utilities/enumerator.h"

namespace genebits::engine
{

enum class AttachmentType
{
  None = 0,
  Depth = BitFlag(0),
  Color = BitFlag(1),
  Present = BitFlag(2) | BitFlag(1),
};

DEFINE_ENUM_FLAG_OPERATORS(AttachmentType)

}

#endif // GENEBITS_VULKAN_ATTACHMENT_TYPE_H
