#include "plex/utilities/type_sort.h"

namespace plex::tests
{
namespace
{
  template<size_t Tag>
  struct TestType
  {};

  // Archetype generation util
  template<size_t... Tags>
  using ATG = SortedTypes<TestType<Tags>...>;
} // namespace

// Archetype static tests

// We could just make a permutation generator, but we should keep these assertions because
// it's easier to read if ever there is an error.

static_assert(std::is_same_v<ATG<0>, ATG<0>>);
static_assert(!std::is_same_v<ATG<0>, ATG<1>>);
static_assert(!std::is_same_v<ATG<1>, ATG<0>>);

static_assert(!std::is_same_v<ATG<1, 2>, ATG<0>>);
static_assert(!std::is_same_v<ATG<1>, ATG<0, 1>>);

static_assert(std::is_same_v<ATG<0, 0>, ATG<0, 0>>);
static_assert(std::is_same_v<ATG<1, 0>, ATG<1, 0>>);
static_assert(std::is_same_v<ATG<0, 1>, ATG<1, 0>>);
static_assert(std::is_same_v<ATG<0, 1>, ATG<0, 1>>);
static_assert(std::is_same_v<ATG<1, 0>, ATG<0, 1>>);
static_assert(!std::is_same_v<ATG<0, 0>, ATG<1, 0>>);
static_assert(!std::is_same_v<ATG<0, 1>, ATG<0, 0>>);

static_assert(!std::is_same_v<ATG<0, 0, 0>, ATG<0, 0>>);
static_assert(!std::is_same_v<ATG<0, 0>, ATG<0, 0, 0>>);
static_assert(!std::is_same_v<ATG<0, 0>, ATG<0, 1, 0>>);
static_assert(!std::is_same_v<ATG<0, 2>, ATG<0, 1, 0>>);

static_assert(std::is_same_v<ATG<0, 0, 0>, ATG<0, 0, 0>>);
static_assert(!std::is_same_v<ATG<0, 1, 0>, ATG<0, 0, 0>>);
static_assert(!std::is_same_v<ATG<0, 0, 0>, ATG<0, 0, 1>>);
static_assert(!std::is_same_v<ATG<1, 0, 0>, ATG<0, 0, 0>>);
static_assert(std::is_same_v<ATG<1, 0, 0>, ATG<1, 0, 0>>);
static_assert(std::is_same_v<ATG<1, 0, 0>, ATG<0, 1, 0>>);
static_assert(std::is_same_v<ATG<1, 0, 0>, ATG<0, 0, 1>>);
static_assert(std::is_same_v<ATG<0, 1, 0>, ATG<1, 0, 0>>);
static_assert(std::is_same_v<ATG<0, 1, 0>, ATG<0, 1, 0>>);
static_assert(std::is_same_v<ATG<0, 1, 0>, ATG<0, 0, 1>>);
static_assert(std::is_same_v<ATG<0, 0, 1>, ATG<1, 0, 0>>);
static_assert(std::is_same_v<ATG<0, 0, 1>, ATG<0, 1, 0>>);
static_assert(std::is_same_v<ATG<0, 0, 1>, ATG<0, 0, 1>>);
static_assert(!std::is_same_v<ATG<1, 0, 1>, ATG<0, 0, 1>>);
static_assert(!std::is_same_v<ATG<0, 1, 1>, ATG<0, 0, 1>>);
static_assert(!std::is_same_v<ATG<1, 1, 0>, ATG<0, 0, 1>>);
static_assert(!std::is_same_v<ATG<1, 0, 0>, ATG<0, 1, 1>>);
static_assert(!std::is_same_v<ATG<1, 0, 0>, ATG<1, 1, 0>>);
static_assert(std::is_same_v<ATG<1, 1, 0>, ATG<1, 1, 0>>);
static_assert(std::is_same_v<ATG<1, 0, 1>, ATG<1, 1, 0>>);
static_assert(std::is_same_v<ATG<1, 0, 1>, ATG<0, 1, 1>>);
static_assert(std::is_same_v<ATG<1, 0, 1>, ATG<1, 0, 1>>);
static_assert(std::is_same_v<ATG<1, 1, 0>, ATG<1, 0, 1>>);
static_assert(std::is_same_v<ATG<0, 1, 1>, ATG<1, 0, 1>>);
static_assert(std::is_same_v<ATG<0, 1, 1>, ATG<1, 1, 0>>);
static_assert(!std::is_same_v<ATG<1, 1, 1>, ATG<1, 1, 0>>);
static_assert(!std::is_same_v<ATG<2, 1, 1>, ATG<1, 1, 0>>);
static_assert(!std::is_same_v<ATG<2, 1, 1>, ATG<1, 1, 3>>);
static_assert(!std::is_same_v<ATG<2, 1, 1>, ATG<1, 4, 3>>);
static_assert(std::is_same_v<ATG<3, 4, 1>, ATG<1, 4, 3>>);
static_assert(std::is_same_v<ATG<3, 5, 1>, ATG<1, 5, 3>>);
static_assert(std::is_same_v<ATG<1, 5, 3>, ATG<5, 1, 3>>);

static_assert(std::is_same_v<ATG<1, 2, 3, 4>, ATG<1, 2, 3, 4>>);
static_assert(std::is_same_v<ATG<4, 3, 2, 1>, ATG<1, 2, 3, 4>>);
static_assert(std::is_same_v<ATG<1, 5, 3, 7>, ATG<5, 7, 1, 3>>);
static_assert(!std::is_same_v<ATG<2, 5, 3, 7>, ATG<5, 7, 1, 3>>);

static_assert(std::is_same_v<ATG<1, 2, 3, 4, 5>, ATG<1, 2, 3, 4, 5>>);
static_assert(std::is_same_v<ATG<5, 4, 3, 2, 1>, ATG<1, 2, 3, 4, 5>>);
static_assert(std::is_same_v<ATG<1, 5, 10, 3, 7>, ATG<5, 7, 1, 3, 10>>);
static_assert(!std::is_same_v<ATG<2, 11, 5, 3, 7>, ATG<5, 7, 1, 10, 3>>);

static_assert(std::is_same_v<ATG<1, 2, 3, 4, 5, 6>, ATG<1, 2, 3, 4, 5, 6>>);
static_assert(std::is_same_v<ATG<6, 5, 4, 3, 2, 1>, ATG<1, 2, 3, 4, 5, 6>>);
static_assert(std::is_same_v<ATG<1, 5, 2, 10, 3, 15, 7>, ATG<2, 5, 7, 1, 3, 10, 15>>);
static_assert(!std::is_same_v<ATG<2, 11, 5, 3, 7, 15>, ATG<5, 15, 7, 1, 10, 3>>);

static_assert(std::is_same_v<ATG<1, 2, 3, 4, 5, 6, 7>, ATG<1, 2, 3, 4, 5, 6, 7>>);
static_assert(std::is_same_v<ATG<7, 6, 5, 4, 3, 2, 1>, ATG<1, 2, 3, 4, 5, 6, 7>>);
static_assert(std::is_same_v<ATG<1, 5, 2, 10, 3, 15, 7, 16>, ATG<2, 16, 5, 7, 1, 3, 10, 15>>);
static_assert(!std::is_same_v<ATG<2, 11, 5, 3, 16, 7, 15>, ATG<5, 15, 8, 1, 10, 3, 16>>);

static_assert(std::is_same_v<ATG<1, 2, 3, 4, 5, 6, 7, 8>, ATG<1, 2, 3, 4, 5, 6, 7, 8>>);
static_assert(std::is_same_v<ATG<8, 7, 6, 5, 4, 3, 2, 1>, ATG<1, 2, 3, 4, 5, 6, 7, 8>>);

static_assert(std::is_same_v<ATG<1, 2, 3, 4, 5, 6, 7, 8, 9>, ATG<1, 2, 3, 4, 5, 6, 7, 8, 9>>);
static_assert(std::is_same_v<ATG<9, 8, 7, 6, 5, 4, 3, 2, 1>, ATG<1, 2, 3, 4, 5, 6, 7, 8, 9>>);

static_assert(std::is_same_v<ATG<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>, ATG<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>>);
static_assert(std::is_same_v<ATG<10, 9, 8, 7, 6, 5, 4, 3, 2, 1>, ATG<1, 2, 3, 4, 5, 6, 7, 8, 9, 10>>);

static_assert(std::is_same_v<ATG<1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11>, ATG<1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11>>);
static_assert(std::is_same_v<ATG<11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>, ATG<1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11>>);

static_assert(std::is_same_v<ATG<1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12>, ATG<1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12>>);
static_assert(std::is_same_v<ATG<12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1>, ATG<1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12>>);
static_assert(!std::is_same_v<ATG<12, 11, 10, 9, 99, 7, 6, 5, 4, 3, 2, 1>, ATG<1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12>>);

} // namespace plex::tests
