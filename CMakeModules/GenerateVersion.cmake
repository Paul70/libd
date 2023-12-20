find_package(Git)

message(STATUS "GenerateVersion.cmake: current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")

# source dir is extracted from parameter.
execute_process(COMMAND ${GIT_EXECUTABLE} --version
    WORKING_DIRECTORY ${SOURCE_DIR}
    OUTPUT_VARIABLE LIBD_CURRENT_GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --always
    WORKING_DIRECTORY ${SOURCE_DIR}
    OUTPUT_VARIABLE LIBD_LATEST_GIT_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "GenerateVersion.cmake: latest Git tag: ${LIBD_LATEST_GIT_TAG}")

execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY ${SOURCE_DIR}
    OUTPUT_VARIABLE LIBD_CURRENT_GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "GenerateVersion.cmake: current Git commit hash: ${LIBD_CURRENT_GIT_COMMIT_HASH}")

execute_process(COMMAND ${GIT_EXECUTABLE} log -n1 --date=iso --pretty=format:%ad
    WORKING_DIRECTORY ${SOURCE_DIR}
    OUTPUT_VARIABLE LIBD_CURRENT_GIT_COMMIT_TIME
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "GenerateVersion.cmake: current Git commit time: ${LIBD_CURRENT_GIT_COMMIT_TIME}")

# Input and output files are extracted from parameters.
configure_file(${INPUT_FILE} ${OUTPUT_FILE})
