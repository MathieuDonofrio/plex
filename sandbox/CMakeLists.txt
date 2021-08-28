#
# Test
#

#
# Utilities
#

set(engine_include_directories ${CMAKE_CURRENT_SOURCE_DIR}/../engine/include)

function(add_sandbox_project subdirectory)
    message(STATUS "Adding Sandbox Project: " ${subdirectory})

    file(GLOB_RECURSE ${subdirectory}_sandbox_headers ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory}/*.h)
    file(GLOB_RECURSE ${subdirectory}_sandbox_headers ${CMAKE_CURRENT_SOURCE_DIR}/${subdirectory}/*.cpp)

    add_executable(${subdirectory}_sandbox ${${subdirectory}_sandbox_headers} ${${subdirectory}_sandbox_headers})

    target_include_directories(${subdirectory}_sandbox PRIVATE ${engine_include_directories})
    target_link_libraries(${subdirectory}_sandbox PRIVATE engine)
endfunction()

#
# Sandboxes
#

add_sandbox_project(basic_window)
add_sandbox_project(logging)