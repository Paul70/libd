### ************************************* Notification ********************************************
### This file should be called from the top level project and it mainly reorganises the cmake
### build directory. Do not load it through cmake's add_subdirectory command.
### ***********************************************************************************************

# Put all runtime, library and archive output artifacts in the same binary folder.
# This is especially helpful for Windows based builds.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
