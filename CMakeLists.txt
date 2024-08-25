cmake_minimum_required(VERSION 3.13)

if(NOT PROJECT_NAME)
    project(libd
            VERSION 0.1.0
            DESCRIPTION "C++ infrastructure library collection"
            LANGUAGES C CXX
    )
endif()

message(STATUS "${PROJECT_NAME}: Loading Libd cmake modules.")
set(D_PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR})

# Get variable that indicates if Libw is a standalone build or part of a top level project
get_directory_property(HAS_PARENT PARENT_DIRECTORY)

# Run configuration steps and include helper functions
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

if(NOT HAS_PARENT)
    include(LibdConfig.cmake)
    include(LibdToolchainConfig.cmake)
endif()

if(BUILD_LIBD_TESTS)
    message(STATUS "Libd: option BUILD_LIBD_TESTS = ON")
endif()
if(BUILD_LIBD_PYTHON)
    message(STATUS "Libd: option BUILD_LIBD_PYTHON = ON")
endif()

if(D_USE_PYTHON)
    if(D_USE_PYTHON STREQUAL "3")
        if(D_TOOLCHAIN STREQUAL "D_GCC" )
            if(D_DEBUG)
                set(Python3_FIND_ABI "ON" "ANY" "ANY")
            else()
                set(Python3_FIND_ABI "OFF" "ANY" "ANY")
            endif()
            find_package(Python REQUIRED COMPONENTS Interpreter Development)
            set(PYTHON3_INCLUDE_DIRS ${Python_INCLUDE_DIRS})
            set(PYTHON3_LIBRARIES ${Python_LIBRARIES})
        else()
            message(FATAL_ERROR "Libd: Python not supported for toolchain.")
            return()
        endif()
    endif()
endif()

# Propagate to parent scope
if(HAS_PARENT)
    set(PYTHON3_INCLUDE_DIRS ${PYTHON3_INCLUDE_DIRS} PARENT_SCOPE)
    set(PYTHON3_LIBRARIES ${PYTHON3_LIBRARIES} PARENT_SCOPE)
endif()

add_subdirectory(libdversion)
add_subdirectory(libdglobals)
add_subdirectory(libdutil)
if(D_USE_PYTHON)
    add_subdirectory(libdpython)
endif()
if(BUILD_LIBD_TESTS)
    add_subdirectory(tests)
endif()







