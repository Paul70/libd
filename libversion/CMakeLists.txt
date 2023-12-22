add_library(dversion INTERFACE)

# Call GenerateVersion.cmake and create a target that has built-in version information.
add_custom_target(LibdGitVersion
    COMMAND ${CMAKE_COMMAND}
        -D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
        -D INPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
        -D OUTPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/version.h
        -P ${CMAKE_CURRENT_SOURCE_DIR}/../CMakeModules/GenerateVersion.cmake
    COMMENT "libversion CMakeLists.txt: building dversion target to include version.h."
)

add_dependencies(dversion LibdGitVersion)
target_include_directories(dversion INTERFACE ${CMAKE_CURRENT_BINARY_DIR})


