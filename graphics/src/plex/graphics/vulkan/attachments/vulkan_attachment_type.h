#ifndef GENEBITS_VULKAN_ATTACHMENT_TYPE_H
#define GENEBITS_VULKAN_ATTACHMENT_TYPE_H

#include "plex/utilities/enumerator.h"

namespace plex
{

enum class AttachmentType
{
  None = 0,
  Depth = BitFlag(0),
  Color = BitFlag(1),
  Present = BitFlag(2) | BitFlag(1),
};

DEFINE_ENUM_FLAG_OPERATORS(AttachmentType)

} // namespace plex

#endif
