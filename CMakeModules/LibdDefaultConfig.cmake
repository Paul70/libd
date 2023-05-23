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

# Set variable to identify the compiler tool chain. It is also possible to use the value of this
# variable as a preprocessor arguement (add_compile_definition) for writing compiler specific code sections.
# Example: #ifdef D_GCC ***compiler sepcific code*** #endif
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(D_TOOLCHAIN "D_GCC")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(D_TOOLCHAIN "D_CLANG")
    message(FATAL_ERROR "Configuration for Clang compiler toolchain currently not implemennted, use GNU instead.")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(D_TOOLCHAIN "D_MSVC")
    message(FATAL_ERROR "Configuration for MSVC compiler toolchain currently not implemennted, use GNU instead.")
else()
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER_ID} is currently not supported. Libd is designed for either GNU or Clang or MSVC.")
endif()
add_compile_definitions(${D_TOOLCHAIN})

# C/C++ language standards
set_c_cxx_configuration(D_TOOLCHAIN USE_CXX_20)

# Set preprocessor build type variable D_DEBUG.
if(NOT CMAKE_BUILD_TYPE)
    set(D_DEBUG OFF)
    message(AUTHOR_WARNING "Defaulting build type to Release since build type variable is not set.")
elseif(CMAKE_BUILD_TYPE STREQUAL Release OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo OR CMAKE_BUILD_TYPE STREQUAL MinSizeRel)
    set(D_DEBUG OFF)
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
else()
    set(D_DEBUG ON)
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()
add_compile_definitions(D_DEBUG)

# Compiler options and configurations.
if(${D_TOOLCHAIN} STREQUAL "D_GCC")
    set(D_DEFAULT_BUILD_SHARED_LIBS ON)
    # -Wall: enable all warnings
    # -Wextra: enable extra warnings not coverd by Wall option
    # -Werror: treat all warnings as errors
    # -O0: disable many compiler optimizations (optimization lavel 0: Reduce compilation time and make debuggind produce the expected resluts.)
    # -Wfloat-conversion: warn for implicit conversions
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wfloat-conversion -Wall -Werror -Wextra -O0")
    # -02: turn on optimization level 2 (of 3)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wfloat-conversion -Werror -Wall -Wextra -O2")
elseif(${D_TOOLCHAIN} STREQUAL "D_CLANG")
    set(D_DEFAULT_BUILD_SHARED_LIBS ON)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wfloat-conversion -Wall -Werror -Wextra -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wfloat-conversion -Werror -Wall -Wextra -O2")
else()
    # to be done for msvc.
endif()

# When building the code as a static library, set -fpic compiler option which produces postion independent
# code. This is a prerequisite for using the static lib in another shared lib.
if(NOT D_DEFAULT_BUILD_SHARED_LIBS)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fpic")
endif()


