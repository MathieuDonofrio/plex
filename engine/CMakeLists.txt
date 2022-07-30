#
# Engine
#

#
# Library
#

file(GLOB_RECURSE engine_headers ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h)
file(GLOB_RECURSE engine_sources ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_library(engine STATIC ${engine_sources})

set(engine_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(engine_source_directories ${CMAKE_CURRENT_SOURCE_DIR}/source)

target_include_directories(engine PRIVATE ${engine_include_directories})
target_include_directories(engine PRIVATE ${engine_source_directories})

target_compile_features(engine INTERFACE cxx_std_20)

set_project_warnings(engine)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(vulkan-memory-allocator REQUIRED)

target_include_directories(engine PRIVATE ${glfw_INCLUDE_DIRS} ${Vulkan_INCLUDE_DIRS})
target_link_libraries(engine ${glfw_LIBRARIES} ${Vulkan_LIBRARIES})
target_link_libraries(engine vulkan-memory-allocator::vulkan-memory-allocator)
#
# Tests
#

if (BUILD_TESTS)
  add_subdirectory(test)
endif ()

#
# Benchmarks
#

if (BUILD_BENCHMARKS)
  add_subdirectory(benchmark)
endif ()
