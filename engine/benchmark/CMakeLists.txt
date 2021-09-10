#
# Test
#

#
# Utilities
# 

find_package(benchmark REQUIRED)

function(add_engine_benchmark subdirectory)
    message(STATUS "Adding Benchmark: " ${subdirectory})

    file(GLOB_RECURSE ${subdirectory}_test_headers ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory}/*.h)
    file(GLOB_RECURSE ${subdirectory}_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory}/*.cpp)

    add_executable(${subdirectory}_benchmark ${${subdirectory}_test_headers} ${${subdirectory}_test_sources})

    target_include_directories(${subdirectory}_benchmark PRIVATE ${engine_include_directories} ${benchmark_INCLUDE_DIRS})
    target_link_libraries(${subdirectory}_benchmark PRIVATE engine ${benchmark_LIBRARIES})
endfunction()

#
# Benchmarks
#

add_engine_benchmark(util)
add_engine_benchmark(events)
add_engine_benchmark(graphics)
add_engine_benchmark(ecs)
add_engine_benchmark(jobs)
