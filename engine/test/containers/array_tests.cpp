#include "genebits/engine/containers/array.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
static_assert(Array<int, 0>::Size == 0);
static_assert(Array<int, 1>::Size == 1);
static_assert(Array<int, 1000>::Size == 1000);
static_assert(ConcatArrays<int>().Size == 0);
static_assert(ConcatArrays<int>(Array<int, 3> { 1, 2, 3 }, Array<int, 2> { 1, 2 }).Size == 5);

TEST(Array_Tests, ConcatArrays_NoArrays_ZeroSizedArray)
{
  auto result = ConcatArrays<int>();

  EXPECT_EQ(result.size(), 0);
}

TEST(Array_Tests, ConcatArrays_MultipleArrays_Concatenated)
{
  Array<int, 3> a1 { 0, 1, 2 };
  Array<int, 3> a2 { 3, 4, 5 };
  Array<int, 4> a3 { 6, 7, 8, 9 };

  auto result = ConcatArrays<int>(a1, a2, a3);

  EXPECT_EQ(result.size(), 10);

  Array<int, 10> expected { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  EXPECT_EQ(result, expected);
}
} // namespace genebits::engine::tests