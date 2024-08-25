# #############################################################################
#
# Information:
# - custom cmake variables used throughout the build all start with D_...,
#   e.g. D_Debug.
# - CMakeModule files (.cmake) always have to be included in the top
#   level CMakeLists.txt
#
# #############################################################################

#------------------------------------------------------------------------------
# Cmake option definition
# -----------------------------------------------------------------------------
option(BUILD_LIBD_TESTS "Build libd Googletest test suite." ON)
option(BUILD_LIBD_PYTHON "Build libd Googletest test suite." ON)


#------------------------------------------------------------------------------
# Custom cmake cache variables and their allowed propteties
# -----------------------------------------------------------------------------
set(D_PROJECT_DIR "" CACHE STRING "Libd project directory path")

set(D_DEBUG OFF CACHE STRING "Libd debug build")
set_property(CACHE D_DEBUG PROPERTY STRINGS ON OFF)

set(D_RELEASE OFF CACHE STRING "Libd release build")
set_property(CACHE D_RELEASE PROPERTY STRINGS ON OFF)

set(D_TOOLCHAIN FALSE CACHE STRING "Libd toolchain variable - possible values")
set_property(CACHE D_TOOLCHAIN PROPERTY STRINGS FALSE D_GCC)

set(D_BUILD_SHARED_LIBS OFF CACHE STRING "Build components as shared libs")
set_property(CACHE D_BUILD_SHARED_LIBS PROPERTY STRINGS ON OFF)

set(D_USE_PYTHON 3 CACHE STRING "Build optional components for given Python version")
set_property(CACHE D_USE_PYTHON PROPERTY STRINGS 0 2 3)


#------------------------------------------------------------------------------
# Configure build paths
#
# Put all runtime, library and archive output artifacts in the same
# binary folder. This is especially helpful for Windows based builds.
# -----------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message(STATUS "Libd: runtime binaries are put in:  ${CMAKE_BINARY_DIR}/bin")
message(STATUS "Libd: library binaries are put in:  ${CMAKE_BINARY_DIR}/bin")
message(STATUS "Libd: artefact binaries are put in: ${CMAKE_BINARY_DIR}/bin")
