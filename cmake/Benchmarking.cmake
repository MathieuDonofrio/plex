#
# Benchmarking
#

option(BUILD_BENCHMARKS "Enable building with benchmarks" OFF)

if (BUILD_BENCHMARKS)
    message(STATUS "Building with benchmarking")
    add_compile_definitions(BUILD_BENCHMARKS)
endif ()