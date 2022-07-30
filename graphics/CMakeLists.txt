#
# Graphics
#

file(GLOB_RECURSE graphics_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(plex-graphics STATIC ${graphics_sources})

set(graphics_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(graphics_source_directories ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_include_directories(plex-graphics PUBLIC ${graphics_include_directories})
target_include_directories(plex-graphics PRIVATE ${graphics_source_directories})

target_link_libraries(plex-graphics PUBLIC plex-core)

target_compile_features(plex-graphics INTERFACE cxx_std_20)

set_project_warnings(plex-graphics)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)
find_package(vulkan-memory-allocator REQUIRED)

target_include_directories(plex-graphics PRIVATE ${glfw_INCLUDE_DIRS} ${Vulkan_INCLUDE_DIRS})
target_link_libraries(plex-graphics PRIVATE ${glfw_LIBRARIES} ${Vulkan_LIBRARIES} vulkan-memory-allocator::vulkan-memory-allocator)
