#
# Editor
#

option(BUILD_EDITOR "Enable building with editor" OFF)

if(BUILD_EDITOR)
  message(STATUS "Building with editor")
  add_compile_definitions(BUILD_EDITOR)
endif()