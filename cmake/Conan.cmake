#
# Conan
#

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
                TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_check(VERSION 1.37.0 REQUIRED)

set(CONAN_REQUIRED_PACKAGES "")
set(CONAN_PACKAGE_OPTIONS "")

macro(add_conan_package package)
  set(CONAN_REQUIRED_PACKAGES ${CONAN_REQUIRED_PACKAGES} ${package})
endmacro()

macro(add_conan_option option)
  set(CONAN_PACKAGE_OPTIONS ${CONAN_PACKAGE_OPTIONS} ${option})
endmacro()

macro(run_conan)
  conan_cmake_configure(REQUIRES ${CONAN_REQUIRED_PACKAGES}
                        OPTIONS ${CONAN_PACKAGE_OPTIONS}
                        GENERATORS cmake_find_package)

  conan_cmake_autodetect(settings)

  conan_cmake_install(PATH_OR_REFERENCE .
                      BUILD missing
                      REMOTE conan-center
                      SETTINGS ${settings})
endmacro()