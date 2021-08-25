#
# Debugging
#

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    option(BUILD_DEBUGGING "Enable building with debugging" OFF)
else ()
    option(BUILD_DEBUGGING "Enable building with debugging" ON)
endif ()

if (BUILD_DEBUGGING)
    message(STATUS "Building with debugging")

    # MSVC doesn't like _DEBUG on release builds. See PR 4379.
    if (NOT MSVC)
        add_definitions(-D_DEBUG)
    endif ()

    # Undef NDEBUG
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_definitions(-UNDEBUG)
    endif ()
    if (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        add_definitions(-UNDEBUG)
    endif ()
else ()
    message(STATUS "Building without debugging")

    # Define NDEBUG
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_definitions(-DNDEBUG)
    endif ()
endif ()
