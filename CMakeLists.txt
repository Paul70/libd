cmake_minimum_required(VERSION 3.13)

if(NOT PROJECT_NAME)
    project(libd
            VERSION 0.1.0
            DESCRIPTION "C++ developement infrastructure library"
            LANGUAGES C CXX)
endif()

# Global options for libd project. Have to be set before loading cmake modules.
option(BUILD_LIBD_TESTS "Build libd Googletest test suite and use Googletest system installation" ON)
option(BUILD_LIBD_TESTS_AND_GOOGLETEST "Build libd Googletest test suite and Googletest framework itsself" OFF)
option(USE_CXX_20 "Enable C++ 20 features. Option gets deactivated if the compiler does not support C++ 20." OFF)

# Check for parent cmake projects.
get_directory_property(HAS_PARENT PARENT_DIRECTORY)

if(NOT HAS_PARENT)
    # If this is the top leve CmakeLists.txt then include Cmake modules.
    # That should always happen in the top most project.
    message(STATUS "Loading CMake modules")
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)
    include(HelperFunctions)
    include(LibdDefaultConfig)
    include(ConfigureBuildPaths)
endif()

# Internal options depending on LibdDefaultConfig module
option(BUILD_SHARED_LIBS "Build all libraries as shared libs unless it is defined ohterwise" ${D_DEFAULT_BUILD_SHARED_LIBS})

add_subdirectory(libdglobals)
add_subdirectory(libdutil)

if(BUILD_LIBD_TESTS)
    set(BUILD_LIBD_TESTS_AND_GOOGLETEST OFF)
    add_subdirectory(tests)
endif()
if(BUILD_LIBD_TESTS_AND_GOOGLETEST)
    set(BUILD_LIBD_TESTS OFF)
    add_compile_options(-Wno-error)
    add_subdirectory(thirdparty/googletest)
    add_subdirectory(tests)
endif()





