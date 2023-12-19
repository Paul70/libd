### ************************************* Notification ********************************************
### This file should be called from the top level project. Do not load it through
### cmake's add_subdirectory command. There are issues with add_compile_definitions in
### combination with add_subdirectory.
### ***********************************************************************************************
cmake_minimum_required(VERSION 3.13)

# Guard against multiple includes
if(D_TOOLCHAIN)
    return()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(D_DEBUG ON)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(D_DEBUG OFF)
endif()
add_compile_definitions(D_DEBUG)

message(STATUS "LibdDefaultConfig.cmake: build type is: ${CMAKE_BUILD_TYPE}")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(D_TOOLCHAIN "D_GCC")
else()
    set(D_TOOLCHAIN FALSE)
    message(FATAL_ERROR "LibdDefaultConfig.cmake: compiler ${CMAKE_CXX_COMPILER_ID} toolchain not supported.")
endif()
add_compile_definitions(${D_TOOLCHAIN})

# Set compiler flags.
# -Wall: enable all warnings
# -Wextra: enable extra warnings not coverd by Wall option
# -Werror: treat all warnings as errors
# -O0: disable many compiler optimizations (optimization lavel 0: Reduce compilation time and make debuggind produce the expected resluts.)
# -Wfloat-conversion: warn for implicit conversions
# -02: turn on optimization level 2 (of 3)
if(${D_TOOLCHAIN} STREQUAL "D_GCC")
    set(D_DEFAULT_BUILD_SHARED_LIBS ON)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-dangling-reference -Wfloat-conversion -Wall -Werror -Wextra -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-dangling-reference -Wfloat-conversion -Werror -Wall -Wextra -O2")
endif()

if(D_DEBUG)
    message(STATUS "LibdDefaultConfig.cmake: used compiler flags are: ${CMAKE_CXX_FLAGS_DEBUG}")
else()
    message(STATUS "LibdDefaultConfig.cmake: used compiler flags are: ${CMAKE_CXX_FLAGS_RELEASE}")
endif()

# When building the code as a static library, set -fpic compiler option which produces postion independent
# code. This is a prerequisite for using the static lib in another shared lib.
if(NOT D_DEFAULT_BUILD_SHARED_LIBS)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fpic")
endif()


