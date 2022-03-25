#include "genebits/engine/math/mat2x2.h"

#include <gtest/gtest.h>

namespace genebits::engine::tests
{

static_assert(std::is_trivially_copy_constructible_v<float2x2>, "Mat2x2 Must be trivially copy constructable");
static_assert(std::is_trivially_move_constructible_v<float2x2>, "Mat2x2 Must be trivially move constructable");
static_assert(std::is_trivially_destructible_v<float2x2>, "Mat2x2 Must be trivially move constructable");
static_assert(std::is_trivially_copy_assignable_v<float2x2>, "Mat2x2 Must be trivially copy assignable");
static_assert(std::is_trivially_move_assignable_v<float2x2>, "Mat2x2 Must be trivially move assignable");

static_assert(std::is_trivially_copyable_v<float2x2>, "Mat2x2 Must be trivially copyable");

TEST(Mat2x2_Tests, PROTOTYPE) {}

} // namespace genebits::engine::tests