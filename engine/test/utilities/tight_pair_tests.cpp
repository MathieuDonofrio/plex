#include "genebits/engine/utilities/tight_pair.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{
namespace
{
  template<typename First, typename Second>
  struct RegularPair
  {
    First first;
    Second second;
  };

  struct EmptyClass1
  {};

  struct EmptyClass2
  {};
} // namespace

static_assert(sizeof(TightPair<int, int>) == sizeof(RegularPair<int, int>));
static_assert(sizeof(TightPair<int, EmptyClass1>) == sizeof(int));
static_assert(sizeof(TightPair<EmptyClass1, int>) == sizeof(int));
static_assert(sizeof(TightPair<EmptyClass1, EmptyClass2>) == sizeof(unsigned char));
static_assert(sizeof(TightPair<int, double>) == sizeof(RegularPair<int, double>));
static_assert(sizeof(TightPair<double, int>) == sizeof(RegularPair<double, int>));

TEST(TightTuple_Tests, DefaultConstructor_DefaultBothNonEmpty_DefaultValues)
{
  TightPair<int, int> tuple {};

  EXPECT_EQ(tuple.First(), 0);
  EXPECT_EQ(tuple.Second(), 0);

  const TightPair<int, int> const_tuple {};

  EXPECT_EQ(const_tuple.First(), 0);
  EXPECT_EQ(const_tuple.Second(), 0);
}

TEST(TightTuple_Tests, DefaultConstructor_DefaultFirstEmpty_DefaultValues)
{
  TightPair<EmptyClass1, int> tuple {};

  EXPECT_NO_FATAL_FAILURE([[maybe_unused]] auto& r = tuple.First());
  EXPECT_EQ(tuple.Second(), 0);

  const TightPair<EmptyClass1, int> const_tuple {};

  EXPECT_NO_FATAL_FAILURE([[maybe_unused]] auto& r = const_tuple.First());
  EXPECT_EQ(const_tuple.Second(), 0);
}

TEST(TightTuple_Tests, DefaultConstructor_SecondEmpty_DefaultValues)
{
  TightPair<int, EmptyClass1> tuple {};

  EXPECT_EQ(tuple.First(), 0);
  EXPECT_NO_FATAL_FAILURE([[maybe_unused]] auto& r = tuple.Second());

  const TightPair<int, EmptyClass1> const_tuple {};

  EXPECT_EQ(const_tuple.First(), 0);
  EXPECT_NO_FATAL_FAILURE([[maybe_unused]] auto& r = const_tuple.Second());
}

TEST(TightTuple_Tests, DefaultConstructor_BothEmpty_NoFail)
{
  TightPair<EmptyClass1, EmptyClass2> tuple {};

  EXPECT_NO_FATAL_FAILURE([[maybe_unused]] auto& r = tuple.First());
  EXPECT_NO_FATAL_FAILURE([[maybe_unused]] auto& r = tuple.Second());

  const TightPair<EmptyClass1, EmptyClass2> const_tuple {};

  EXPECT_NO_FATAL_FAILURE([[maybe_unused]] auto& r = const_tuple.First());
  EXPECT_NO_FATAL_FAILURE([[maybe_unused]] auto& r = const_tuple.Second());
}

TEST(TightTuple_Tests, ParametricConstructor_BothNonEmpty_DefaultValues)
{
  TightPair<int, int> tuple { 1, 2 };

  EXPECT_EQ(tuple.First(), 1);
  EXPECT_EQ(tuple.Second(), 2);

  const TightPair<int, int> const_tuple { 50, 100 };

  EXPECT_EQ(const_tuple.First(), 50);
  EXPECT_EQ(const_tuple.Second(), 100);
}

TEST(TightTuple_Tests, ParametricConstructor_LeftEmpty_DefaultValues)
{
  TightPair<EmptyClass1, int> tuple { {}, 10 };

  EXPECT_EQ(tuple.Second(), 10);

  const TightPair<EmptyClass1, int> const_tuple { {}, 50 };

  EXPECT_EQ(const_tuple.Second(), 50);
}

TEST(TightTuple_Tests, ParametricConstructor_RightEmpty_DefaultValues)
{
  TightPair<int, EmptyClass1> tuple { 10, EmptyClass1 {} };

  EXPECT_EQ(tuple.First(), 10);

  const TightPair<int, EmptyClass1> const_tuple { 50, EmptyClass1 {} };

  EXPECT_EQ(const_tuple.First(), 50);
}
} // namespace genebits::engine::tests