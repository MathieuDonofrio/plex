#
# Testing
#

option(BUILD_TESTS "Enable building with tests" OFF)

if(BUILD_TESTS)
  message(STATUS "Building with testing")
  add_compile_definitions(BUILD_TESTS)
  include(CTest)
  enable_testing()
endif()
