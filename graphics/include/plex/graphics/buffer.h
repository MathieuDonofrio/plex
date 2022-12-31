#ifndef PLEX_GRAPHICS_BUFFER_H
#define PLEX_GRAPHICS_BUFFER_H

#include "plex/utilities/enum_flag.h"

namespace plex::graphics
{
// TODO

enum class BufferUsageFlags
{
  Vertex = BitFlag(0),
  Index = BitFlag(1),
  Uniform = BitFlag(2),
  Storage = BitFlag(3),
  All = Vertex | Index | Uniform | Storage,
  None = BitMax(BufferUsageFlags),
};

DEFINE_ENUM_FLAG_OPERATORS(BufferUsageFlags);

class Buffer
{
public:
  constexpr Buffer(size_t size, BufferUsageFlags usage) : size_(size), usage_(usage) {}

  virtual ~Buffer() = default;

  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;
  Buffer(Buffer&&) = delete;
  Buffer& operator=(Buffer&&) = delete;

  ///
  /// Map the buffer to the host (cpu) memory.
  ///
  /// @tparam T The type of the data to map.
  ///
  /// @return Pointer to the mapped memory.
  ///
  template<typename T>
  T* Map()
  {
    return static_cast<T*>(Map());
  }

  ///
  /// Map the buffer to the host (cpu) memory.
  ///
  /// @return Pointer to the mapped memory.
  ///
  virtual void* Map() = 0;

  ///
  /// Unmap the buffer from the host (cpu) memory.
  ///
  virtual void Unmap() = 0;

  constexpr BufferUsageFlags GetUsage() const
  {
    return usage_;
  }

  constexpr size_t size() const
  {
    return size_;
  }

private:
  size_t size_;
  BufferUsageFlags usage_;
};

} // namespace plex::graphics
#endif
