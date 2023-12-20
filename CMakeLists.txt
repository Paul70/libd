# #############################################################################
#
# Information:
# - functions starting with libd_... are non cmake functions and located in
#   module HelperFunctions.cmake.
# - custom cmake variables used thoughout the build all start with D_...,
#   e.g. D_Debug.
#
# #############################################################################
cmake_minimum_required(VERSION 3.13)

if(NOT PROJECT_NAME)
    project(libd
            VERSION 0.1.0
            DESCRIPTION "C++ developement library"
            LANGUAGES C CXX
    )
endif()

# Global options libd project
option(BUILD_LIBD_TESTS "Build libd Googletest test suite and use Googletest system installation" ON)
option(BUILD_LIBD_TESTS_AND_GOOGLETEST "Build libd Googletest test suite and Googletest framework itsself" OFF)

if(BUILD_LIBD_TESTS)
    message(STATUS "libd CMakeLists.txt: option BUILD_LIBD_TESTS = ON")
elseif(BUILD_LIBD_TESTS_AND_GOOGLETEST)
    message(STATUS "libd CMakeLists.txt: option BUILD_LIBD_TESTS_AND_GOOGLETEST on")
endif()

# Check for parent cmake projects.
# Only include cmake modules if this is the top level CMakeLists.txt.
get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(NOT HAS_PARENT)
    message(STATUS "libd CMakeLists.txt: loading CMake modules from: ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
    include(HelperFunctions)
    include(LibdDefaultConfig)
    include(ConfigureBuildPaths)
endif()

add_subdirectory(libversion)
add_subdirectory(libdglobals)
add_subdirectory(libdutil)

if(BUILD_LIBD_TESTS)
    set(BUILD_LIBD_TESTS_AND_GOOGLETEST OFF)
    add_subdirectory(tests)
endif()
if(BUILD_LIBD_TESTS_AND_GOOGLETEST)
    set(BUILD_LIBD_TESTS OFF)
    libd_add_compileflag(-Wno-error "libd CMakeLists.txt")
    add_subdirectory(thirdparty/googletest)
    add_subdirectory(tests)
endif()






