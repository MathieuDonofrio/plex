
option(ENABLE_SIMD "Enable simd" ON)

if (ENABLE_SIMD)
    set(SIMD_FLAGS "")

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(SIMD_FLAGS "/arch:AVX2")
    else ()
        set(SIMD_FLAGS "-mf16c -mavx2 -mfma -mlzcnt -mbmi -mbmi2")
    endif ()

    set(CMAKE_CXX_FLAGS "${SIMD_FLAGS} ${CMAKE_CXX_FLAGS}")
endif ()