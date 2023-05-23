### ************************************* Notification ********************************************
### This module has to be called at first before other modules. Do not load it through
### It defines helper functions used througout the whole cmake configuration code.
### ***********************************************************************************************

# Defining the C/C++ configuration based on the desired standard. The preprocessor identifier
# Function defines the CXX_20 preprocessor identifier to enable code requiering C++20 standard
# features.
function(set_c_cxx_configuration toolchain bool_cxx_20)
    if(${bool_cxx_20} AND ${toolchain} STREQUAL "D_GCC" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 11.0)
        set(CXX_20 ON)
        set(CMAKE_CXX_STANDARD 20)
        add_compile_definitions(CXX_20)
        message(STATUS "CXX standard: C++20")
    elseif(${bool_cxx_20} AND ${toolchain} STREQUAL "D_CLANG" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 11.0)
        set(CXX_20 ON)
        set(CMAKE_CXX_STANDARD 20)
        add_compile_definitions(CXX_20)
        message(STATUS "CXX standard: C++20")
    else()
        set(CMAKE_CXX_STANDARD 17)
        message(STATUS "CXX standard: C++17")
        if(${bool_cxx_20})
            message(AUTHOR_WARNING "Current compiler does not support C++20 or libd does not support C++20 for toolchain ${toolchain}.")
        endif()
    endif()
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED True)
    set(CMAKE_CXX_EXTENSIONS False)
endfunction()
