#
# Micro Benchmarks
#

#
# Utilities
#

find_package(benchmark REQUIRED)

function(add_micro_benchmark subdirectory)
  message(STATUS "Adding micro benchmark: " ${subdirectory})

  file(GLOB_RECURSE ${subdirectory}_bench_headers ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory}/*.h)
  file(GLOB_RECURSE ${subdirectory}_bench_sources ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory}/*.cpp)

  add_executable(bench_${subdirectory} ${${subdirectory}_bench_headers} ${${subdirectory}_bench_sources})

  target_include_directories(bench_${subdirectory} PRIVATE ..)
  target_include_directories(bench_${subdirectory} PRIVATE ${benchmark_INCLUDE_DIRS})

  target_link_libraries(bench_${subdirectory} PRIVATE plex-ecs ${benchmark_LIBRARIES})
endfunction()

#
# Benchmarks
#

add_micro_benchmark(ecs)
