#------------------------------------------------------------------------------
# Build type settings
# -----------------------------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(D_DEBUG ON)
    set(D_RELEASE OFF)
    add_compile_definitions(D_DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(D_DEBUG OFF)
    set(D_RELEASE ON)
    add_compile_definitions(D_RELEASE)
endif()

#------------------------------------------------------------------------------
# Compiler toolchain settings
#
# Used comppiler flags debug/release build GCC:
# -Wall: enable all warnings
# -Wextra: enable extra warnings not coverd by Wall option
# -Werror: treat all warnings as errors
# -O0: disable many compiler optimizations (optimization lavel 0:
#      Reduce compilation time and make debuggind produce the expected resluts.)
# -Wfloat-conversion: warn for implicit conversions
# -02: turn on optimization level 2 (of 3)
# -fpic: when building the code as a static library, set -fpic (postion independent
#        code) to be able to use a static lib in another shared lib.
#
# -----------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(D_TOOLCHAIN "D_GCC")
else()
    set(D_TOOLCHAIN FALSE)
    message(FATAL_ERROR "LibdDefaultConfig.cmake: compiler ${CMAKE_CXX_COMPILER_ID} toolchain not supported.")
    return()
endif()
add_compile_definitions(${D_TOOLCHAIN})

if(D_DEBUG)
    if(${D_TOOLCHAIN} STREQUAL "D_GCC")
        set(D_BUILD_SHARED_LIBS ON)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-dangling-reference -Wfloat-conversion -Wall -Werror -Wextra -O0")

        if(NOT D_BUILD_SHARED_LIBS)
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fpic")
        endif()

        message(STATUS "Libd: used compiler flags are: ${CMAKE_CXX_FLAGS_DEBUG}")
    endif()
else()
    if(${D_TOOLCHAIN} STREQUAL "D_GCC")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-dangling-reference -Wfloat-conversion -Werror -Wall -Wextra -O2")

        if(NOT D_BUILD_SHARED_LIBS)
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fpic")
        endif()

        message(STATUS "Libd: used compiler flags are: ${CMAKE_CXX_FLAGS_RELEASE}")
    endif()
endif()
