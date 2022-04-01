#
# Architecture Optimizations
#

# AVX2 is currently required to use engine
# TODO Allow different architectures to be selected

set(ARCH_FLAGS "")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(ARCH_FLAGS "/arch:AVX2")
else ()
  set(ARCH_FLAGS "-mf16c -mavx2 -mfma -mlzcnt -mbmi -mbmi2")
endif ()

set(CMAKE_CXX_FLAGS "${ARCH_FLAGS} ${CMAKE_CXX_FLAGS}")