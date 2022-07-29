#
# Core
#

file(GLOB_RECURSE core_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(plex-core STATIC ${core_sources})

set(core_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(core_source_directories ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_include_directories(plex-core PUBLIC ${core_include_directories})
target_include_directories(plex-core PRIVATE ${core_source_directories})

target_compile_features(plex-core INTERFACE cxx_std_20)

set_project_warnings(plex-core)

if (BUILD_TESTS)
  add_subdirectory(test)
endif ()

if (BUILD_BENCHMARKS)
  add_subdirectory(bench)
endif ()
