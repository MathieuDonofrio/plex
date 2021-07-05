#
# Debugging
#

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
set(BUILD_DEBUG ON)
else()
set(BUILD_DEBUG OFF)
endif()

if(BUILD_DEBUG)
  message(STATUS "Building with debugging")
  add_compile_definitions(BUILD_DEBUG NDEBUG)
endif()