#
# Engine
#

#
# Library
#

file(GLOB_RECURSE engine_headers ${CMAKE_CURRENT_SOURCE_DIR}/source/*.h)
file(GLOB_RECURSE engine_sources ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_library(engine STATIC ${engine_headers} ${engine_sources})

set(engine_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/config)

target_include_directories(engine PUBLIC ${engine_include_directories})

target_compile_features(engine INTERFACE cxx_std_20)

set_project_warnings(engine)

find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

target_include_directories(engine PUBLIC ${glfw_INCLUDE_DIRS} ${Vulkan_INCLUDE_DIRS})
target_link_libraries(engine ${glfw_LIBRARIES} ${Vulkan_LIBRARIES})

if (BUILD_DEBUG)
    find_package(fmt REQUIRED)

    target_include_directories(engine PUBLIC ${fmt_INCLUDE_DIRS})
    target_link_libraries(engine ${fmt_LIBRARIES})
endif ()

if (BUILD_EDITOR)
    find_package(imgui REQUIRED)

    target_include_directories(engine PUBLIC ${imgui_INCLUDE_DIRS})
    target_link_libraries(engine ${imgui_LIBRARIES})
endif ()

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
