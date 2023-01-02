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

    target_link_libraries(sandbox_${subdirectory} PRIVATE plex-core plex-ecs plex-events plex-graphics)
endfunction()

function(copy_sandbox_directory sandbox path)
    message(STATUS "Copying sandbox directory: " ${sandbox}/${path} " -> " ${path})

    add_custom_command(TARGET sandbox_${sandbox} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/${sandbox}/${path}
            $<TARGET_FILE_DIR:sandbox_${sandbox}>/${path})
endfunction()

#
# Sandboxes
#

add_sandbox_project(logging)
add_sandbox_project(renderer)
add_sandbox_project(systems)
add_sandbox_project(window)