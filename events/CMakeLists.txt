#
# ECS
#

file(GLOB_RECURSE core_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(plex-events STATIC ${core_sources})

set(core_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(core_source_directories ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_include_directories(plex-events PUBLIC ${core_include_directories})
target_include_directories(plex-events PRIVATE ${core_source_directories})

target_compile_features(plex-events INTERFACE cxx_std_20)

target_link_libraries(plex-events PUBLIC plex-core)

set_project_warnings(plex-events)

if (BUILD_TESTS)
  add_subdirectory(test)
endif ()
