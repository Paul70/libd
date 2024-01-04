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

if(BUILD_LIBD_TESTS)
    message(STATUS "libd CMakeLists.txt: option BUILD_LIBD_TESTS = ON")
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

add_subdirectory(libdversion)
add_subdirectory(libdglobals)
add_subdirectory(libdutil)

if(BUILD_LIBD_TESTS)
    add_subdirectory(tests)
endif()







