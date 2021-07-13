#include "engine/util/ring_buffer.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
TEST(RingBuffer_Tests, None_Size_Default)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::None> ring_buffer;

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, None_Enqueue_SizeIncrease)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::None> ring_buffer;

  ring_buffer.Enqueue(10);

  ASSERT_EQ(ring_buffer.Size(), 1u);
  ASSERT_FALSE(ring_buffer.Empty());

  ring_buffer.Enqueue(11);

  ASSERT_EQ(ring_buffer.Size(), 2u);
  ASSERT_FALSE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, None_Clear_SizeIncrease)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::None> ring_buffer;

  ring_buffer.Enqueue(10);

  ring_buffer.Clear();

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());

  ring_buffer.Enqueue(10);
  ring_buffer.Enqueue(11);

  ring_buffer.Clear();

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, None_Dequeue_CorrectValue)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::None> ring_buffer;

  ring_buffer.Enqueue(10);

  int item;

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
  ASSERT_EQ(item, 10);
}

TEST(RingBuffer_Tests, None_Dequeue_CorrectOrder)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::None> ring_buffer;

  ring_buffer.Enqueue(10);
  ring_buffer.Enqueue(11);

  int item;

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 1u);
  ASSERT_FALSE(ring_buffer.Empty());
  ASSERT_EQ(item, 10);

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
  ASSERT_EQ(item, 11);
}

TEST(RingBuffer_Tests, OneToOne_Size_Default)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::OneToOne> ring_buffer;

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, OneToOne_Enqueue_SizeIncrease)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::OneToOne> ring_buffer;

  ring_buffer.Enqueue(10);

  ASSERT_EQ(ring_buffer.Size(), 1u);
  ASSERT_FALSE(ring_buffer.Empty());

  ring_buffer.Enqueue(11);

  ASSERT_EQ(ring_buffer.Size(), 2u);
  ASSERT_FALSE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, OneToOne_Clear_SizeIncrease)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::OneToOne> ring_buffer;

  ring_buffer.Enqueue(10);

  ring_buffer.Clear();

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());

  ring_buffer.Enqueue(10);
  ring_buffer.Enqueue(11);

  ring_buffer.Clear();

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, OneToOne_Dequeue_CorrectValue)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::OneToOne> ring_buffer;

  ring_buffer.Enqueue(10);

  int item;

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
  ASSERT_EQ(item, 10);
}

TEST(RingBuffer_Tests, OneToOne_Dequeue_CorrectOrder)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::OneToOne> ring_buffer;

  ring_buffer.Enqueue(10);
  ring_buffer.Enqueue(11);

  int item;

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 1u);
  ASSERT_FALSE(ring_buffer.Empty());
  ASSERT_EQ(item, 10);

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
  ASSERT_EQ(item, 11);
}

TEST(RingBuffer_Tests, OneToMany_Size_Default)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::OneToMany> ring_buffer;

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, OneToMany_Enqueue_SizeIncrease)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::OneToMany> ring_buffer;

  ring_buffer.Enqueue(10);

  ASSERT_EQ(ring_buffer.Size(), 1u);
  ASSERT_FALSE(ring_buffer.Empty());

  ring_buffer.Enqueue(11);

  ASSERT_EQ(ring_buffer.Size(), 2u);
  ASSERT_FALSE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, OneToMany_Clear_SizeIncrease)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::OneToMany> ring_buffer;

  ring_buffer.Enqueue(10);

  ring_buffer.Clear();

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());

  ring_buffer.Enqueue(10);
  ring_buffer.Enqueue(11);

  ring_buffer.Clear();

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, OneToMany_Dequeue_CorrectValue)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::OneToMany> ring_buffer;

  ring_buffer.Enqueue(10);

  int item;

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
  ASSERT_EQ(item, 10);
}

TEST(RingBuffer_Tests, OneToMany_Dequeue_CorrectOrder)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::OneToMany> ring_buffer;

  ring_buffer.Enqueue(10);
  ring_buffer.Enqueue(11);

  int item;

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 1u);
  ASSERT_FALSE(ring_buffer.Empty());
  ASSERT_EQ(item, 10);

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
  ASSERT_EQ(item, 11);
}

TEST(RingBuffer_Tests, ManyToOne_Size_Default)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::ManyToOne> ring_buffer;

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, ManyToOne_Enqueue_SizeIncrease)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::ManyToOne> ring_buffer;

  ring_buffer.Enqueue(10);

  ASSERT_EQ(ring_buffer.Size(), 1u);
  ASSERT_FALSE(ring_buffer.Empty());

  ring_buffer.Enqueue(11);

  ASSERT_EQ(ring_buffer.Size(), 2u);
  ASSERT_FALSE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, ManyToOne_Clear_SizeIncrease)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::ManyToOne> ring_buffer;

  ring_buffer.Enqueue(10);

  ring_buffer.Clear();

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());

  ring_buffer.Enqueue(10);
  ring_buffer.Enqueue(11);

  ring_buffer.Clear();

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, ManyToOne_Dequeue_CorrectValue)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::ManyToOne> ring_buffer;

  ring_buffer.Enqueue(10);

  int item;

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
  ASSERT_EQ(item, 10);
}

TEST(RingBuffer_Tests, ManyToOne_Dequeue_CorrectOrder)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::ManyToOne> ring_buffer;

  ring_buffer.Enqueue(10);
  ring_buffer.Enqueue(11);

  int item;

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 1u);
  ASSERT_FALSE(ring_buffer.Empty());
  ASSERT_EQ(item, 10);

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
  ASSERT_EQ(item, 11);
}

TEST(RingBuffer_Tests, ManyToMany_Size_Default)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::ManyToMany> ring_buffer;

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, ManyToMany_Enqueue_SizeIncrease)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::ManyToMany> ring_buffer;

  ring_buffer.Enqueue(10);

  ASSERT_EQ(ring_buffer.Size(), 1u);
  ASSERT_FALSE(ring_buffer.Empty());

  ring_buffer.Enqueue(11);

  ASSERT_EQ(ring_buffer.Size(), 2u);
  ASSERT_FALSE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, ManyToMany_Clear_SizeIncrease)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::ManyToMany> ring_buffer;

  ring_buffer.Enqueue(10);

  ring_buffer.Clear();

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());

  ring_buffer.Enqueue(10);
  ring_buffer.Enqueue(11);

  ring_buffer.Clear();

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
}

TEST(RingBuffer_Tests, ManyToMany_Dequeue_CorrectValue)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::ManyToMany> ring_buffer;

  ring_buffer.Enqueue(10);

  int item;

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
  ASSERT_EQ(item, 10);
}

TEST(RingBuffer_Tests, ManyToMany_Dequeue_CorrectOrder)
{
  RingBuffer<int, 16, ConcurrencyGuarantee::ManyToMany> ring_buffer;

  ring_buffer.Enqueue(10);
  ring_buffer.Enqueue(11);

  int item;

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 1u);
  ASSERT_FALSE(ring_buffer.Empty());
  ASSERT_EQ(item, 10);

  ring_buffer.Dequeue(item);

  ASSERT_EQ(ring_buffer.Size(), 0u);
  ASSERT_TRUE(ring_buffer.Empty());
  ASSERT_EQ(item, 11);
}
} // namespace genebits::engine::tests