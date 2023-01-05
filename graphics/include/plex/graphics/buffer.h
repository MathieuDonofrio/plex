#ifndef PLEX_GRAPHICS_BUFFER_H
#define PLEX_GRAPHICS_BUFFER_H

#include "plex/debug/assertion.h"
#include "plex/utilities/enum_flag.h"
#include "plex/utilities/type_traits.h"

namespace plex::graphics
{
enum class BufferUsageFlags
{
  None = 0,
  Vertex = BitFlag(0),
  Index = BitFlag(1),
  Uniform = BitFlag(2),
  Storage = BitFlag(3),
  TransferSource = BitFlag(4),
  TransferDestination = BitFlag(5)
};

DEFINE_ENUM_FLAG_OPERATORS(BufferUsageFlags);

enum class MemoryUsage
{
  Unknown = 0,
  GPU_Only,
  CPU_Only,
  CPU_To_GPU,
  GPU_To_CPU,
  CPU_Copy,
  Auto
};

class PolymorphicBufferInterface
{
public:
  PolymorphicBufferInterface() = default;
  virtual ~PolymorphicBufferInterface() = default;

  PolymorphicBufferInterface(const PolymorphicBufferInterface&) = delete;
  PolymorphicBufferInterface& operator=(const PolymorphicBufferInterface&) = delete;
  PolymorphicBufferInterface(PolymorphicBufferInterface&&) = delete;
  PolymorphicBufferInterface& operator=(PolymorphicBufferInterface&&) = delete;

  virtual void* Map() = 0;
  virtual void Unmap() = 0;

  virtual void* GetNativeHandle() const noexcept = 0;
};

namespace pbi
{
  using Buffer = PolymorphicBufferInterface*;
} // namespace pbi

template<TriviallyRelocatable Type>
class Buffer
{
public:
  constexpr Buffer() : interface_(nullptr), size_(0), buffer_usage_flags_(0), memory_usage_(0) {}

  constexpr Buffer(
    pbi::Buffer interface, uint32_t size, BufferUsageFlags buffer_usage_flags, MemoryUsage memory_usage) noexcept
    : interface_(interface), size_(size), buffer_usage_flags_(static_cast<uint16_t>(buffer_usage_flags)),
      memory_usage_(static_cast<uint16_t>(memory_usage))
  {}

  constexpr Buffer(Buffer&& other) noexcept
    : interface_(other.interface_), size_(other.size_), buffer_usage_flags_(other.buffer_usage_flags_),
      memory_usage_(other.memory_usage_)
  {
    other.interface_ = nullptr;
    other.size_ = 0;
    other.buffer_usage_flags_ = 0;
    other.memory_usage_ = 0;
  }

  Buffer& operator=(Buffer&& other) noexcept
  {
    Buffer(std::move(other)).Swap(*this);
    return *this;
  }

  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;

  ~Buffer()
  {
    if (interface_)
    {
      delete interface_;
    }
  }

  Type* Map()
  {
    ASSERT(interface_, "Buffer is not initialized");

    data_ = static_cast<Type*>(interface_->Map());
    return data_;
  }

  void Unmap()
  {
    ASSERT(interface_, "Buffer is not initialized");

    interface_->Unmap();
  }

  [[nodiscard]] void* GetNativeHandle() const
  {
    return interface_->GetNativeHandle();
  }

  constexpr void Swap(Buffer& other) noexcept
  {
    std::swap(interface_, other.interface_);
    std::swap(size_, other.size_);
    std::swap(buffer_usage_flags_, other.buffer_usage_flags_);
    std::swap(memory_usage_, other.memory_usage_);
  }

  [[nodiscard]] pbi::Buffer GetInterface() const noexcept
  {
    return interface_;
  }

  [[nodiscard]] BufferUsageFlags GetBufferUsageFlags() const noexcept
  {
    return static_cast<BufferUsageFlags>(buffer_usage_flags_);
  }

  [[nodiscard]] MemoryUsage GetMemoryUsage() const noexcept
  {
    return static_cast<MemoryUsage>(memory_usage_);
  }

  [[nodiscard]] Type* data() const noexcept
  {
    return data_;
  }

  [[nodiscard]] constexpr uint32_t size() const noexcept
  {
    return size_;
  }

private:
  pbi::Buffer interface_;
  Type* data_;
  uint32_t size_;
  uint16_t buffer_usage_flags_;
  uint16_t memory_usage_;
};

} // namespace plex::graphics

namespace std
{
template<typename Type>
void swap(plex::graphics::Buffer<Type>& lhs, plex::graphics::Buffer<Type>& rhs) noexcept
{
  lhs.Swap(rhs);
}
} // namespace std
#endif
