#
# Unit Tests
#

#
# Utilities
#

find_package(GTest REQUIRED)

function(add_unit_test subdirectory)
  message(STATUS "Adding unit test: " ${subdirectory})

  file(GLOB_RECURSE ${subdirectory}_test_headers ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory}/*.h)
  file(GLOB_RECURSE ${subdirectory}_test_sources ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory}/*.cpp)

  add_executable(test_${subdirectory} ${${subdirectory}_test_headers} ${${subdirectory}_test_sources})

  target_include_directories(test_${subdirectory} PRIVATE ..)
  target_include_directories(test_${subdirectory} PRIVATE ${core_source_directories} ${GTest_INCLUDE_DIRS})

  target_link_libraries(test_${subdirectory} PRIVATE plex-events ${GTest_LIBRARIES})

  add_test(NAME test_${subdirectory} COMMAND test_${subdirectory})
endfunction()

#
# Tests
#

add_unit_test(events)