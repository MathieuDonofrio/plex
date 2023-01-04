#ifndef PLEX_GRAPHICS_BUFFER_H
#define PLEX_GRAPHICS_BUFFER_H

#include "plex/debug/assertion.h"
#include "plex/utilities/enum_flag.h"
#include "plex/utilities/type_traits.h"

namespace plex::graphics
{
enum class BufferUsageFlags : uint16_t
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

enum class MemoryPropertyFlags : uint16_t
{
  None = 0,
  DeviceLocal = BitFlag(0),
  HostVisible = BitFlag(1),
  HostCoherent = BitFlag(2),
  HostCached = BitFlag(3),
  LazilyAllocated = BitFlag(4)
};

DEFINE_ENUM_FLAG_OPERATORS(MemoryPropertyFlags);

namespace pbi
{
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
} // namespace pbi

template<TriviallyRelocatable Type>
class Buffer
{
public:
  constexpr Buffer()
    : interface_(nullptr), size_(0), usage_(BufferUsageFlags::None), memory_properties_(MemoryPropertyFlags::None)
  {}

  constexpr Buffer(std::unique_ptr<pbi::PolymorphicBufferInterface>&& interface,
    uint32_t size,
    BufferUsageFlags usage,
    MemoryPropertyFlags memory_property) noexcept
    : interface_(interface.release()), size_(size), usage_(usage), memory_properties_(memory_property)
  {}

  constexpr Buffer(Buffer&& other) noexcept
    : interface_(other.interface_), size_(other.size_), usage_(other.usage_),
      memory_properties_(other.memory_properties_)
  {
    other.interface_ = nullptr;
    other.size_ = 0;
    other.usage_ = BufferUsageFlags::None;
    other.memory_properties_ = MemoryPropertyFlags::None;
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
    std::swap(usage_, other.usage_);
    std::swap(memory_properties_, other.memory_properties_);
  }

  [[nodiscard]] BufferUsageFlags GetUsage() const noexcept
  {
    return usage_;
  }

  [[nodiscard]] MemoryPropertyFlags GetMemoryProperties() const noexcept
  {
    return memory_properties_;
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
  pbi::PolymorphicBufferInterface* interface_;
  Type* data_;
  uint32_t size_;
  BufferUsageFlags usage_;
  MemoryPropertyFlags memory_properties_;
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
