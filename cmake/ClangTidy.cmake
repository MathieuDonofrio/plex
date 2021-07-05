#
# Clang-Tidy
#

option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(ENABLE_CLANG_TIDY)
  find_program(CLANGTIDY clang-tidy)
  if(CLANGTIDY)
    message(STATUS "Enabling clang-tidy")
    set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY} -extra-arg=-Wno-unknown-warning-option)
  else()
    message(SEND_ERROR "clang-tidy requested but executable not found")
  endif()
endif()