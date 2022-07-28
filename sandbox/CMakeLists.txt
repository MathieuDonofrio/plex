#
# Test
#

#
# Utilities
#

function(add_sandbox_project subdirectory)
  message(STATUS "Adding sandbox project: " ${subdirectory})

  file(GLOB_RECURSE ${subdirectory}_sandbox_headers ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory}/*.h)
  file(GLOB_RECURSE ${subdirectory}_sandbox_headers ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory}/*.cpp)

  add_executable(sandbox_${subdirectory} ${${subdirectory}_sandbox_headers} ${${subdirectory}_sandbox_headers})

  target_link_libraries(sandbox_${subdirectory} PRIVATE plex-core plex-ecs plex-graphics)
endfunction()

#
# Sandboxes
#

add_sandbox_project(basic_window)
add_sandbox_project(logging)
add_sandbox_project(systems)