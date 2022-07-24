#
# Plex Framework
#

cmake_minimum_required(VERSION 3.20)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Preventing in-source building. Please run cmake in a separate build directory.")
endif ()

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif ()

#
# Project Configuration
#

set(PLEX_VERSION_REGEX "#define PLEX_VERSION_.*[ \t]+(.+)")
file(STRINGS "engine/include/plex/config/version.h" PLEX_VERSION REGEX ${PLEX_VERSION_REGEX})
list(TRANSFORM PLEX_VERSION REPLACE ${PLEX_VERSION_REGEX} "\\1")
string(JOIN "." PLEX_VERSION ${PLEX_VERSION})

project(
  Genebits
  VERSION ${PLEX_VERSION}
  DESCRIPTION "Data-oriented framework"
  HOMEPAGE_URL "https://github.com/MathieuDonofrio/Plex"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_BINARY_DIR})

include(cmake/Debugging.cmake)
include(cmake/Editor.cmake)
include(cmake/Testing.cmake)
include(cmake/Benchmarking.cmake)
include(cmake/ColorDiagnostics.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/ArchitectureOptimizations.cmake)

#
# Install Packages
#

include(cmake/Conan.cmake)

add_conan_package(glfw/3.3.4)

if (BUILD_TESTS)
  add_conan_package(gtest/1.11.0)
endif ()

if (BUILD_BENCHMARKS)
  add_conan_package(benchmark/1.5.3)
endif ()

run_conan()

#
# Engine
#

add_subdirectory(engine)

#
# Sandboxes
#

add_subdirectory(sandbox)
