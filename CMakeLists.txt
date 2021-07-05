#
# Genebits
#

cmake_minimum_required(VERSION 3.17)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Preventing in-source building. Please run cmake in a seperate build directory.")
endif ()

if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif ()

#
# Project Configuration
#

set(GENEBITS_VERSION_REGEX "#define GENEBITS_VERSION_.*[ \t]+(.+)")
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/config/version.h" GENEBITS_VERSION REGEX ${GENEBITS_VERSION_REGEX})
list(TRANSFORM GENEBITS_VERSION REPLACE ${GENEBITS_VERSION_REGEX} "\\1")
string(JOIN "." GENEBITS_VERSION ${GENEBITS_VERSION})

project(
        Genebits
        VERSION ${GENEBITS_VERSION}
        DESCRIPTION "Artificial life simulator"
        HOMEPAGE_URL "https://github.com/MathieuDonofrio/Genebits"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_BINARY_DIR})

include(cmake/Debugging.cmake)
include(cmake/Editor.cmake)
include(cmake/Testing.cmake)
include(cmake/Benchmarking.cmake)
include(cmake/IPO.cmake)
include(cmake/ColorDiagnostics.cmake)
include(cmake/ClangTidy.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/Simd.cmake)

#
# Install Packages
#

include(cmake/Conan.cmake)

add_conan_package(glfw/3.3.4)

if (BUILD_DEBUG)
    add_conan_package(fmt/8.0.0)
endif ()

if (BUILD_EDITOR)
    add_conan_package(imgui/1.83)
endif ()

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