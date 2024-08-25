set(dpython_HEADERS
    pythoninterface.h
)

set(dpython_SOURCES
    pythoninterface.cpp
)

add_library(dpython SHARED)
target_sources(dpython PRIVATE
    ${dpython_HEADERS}
    ${dpython_SOURCES}
)

if(${D_TOOLCHAIN} STREQUAL "D_GCC")
    target_include_directories(dpython PUBLIC ${PYTHON3_INCLUDE_DIRS})
    target_link_libraries(dpython PUBLIC ${PYTHON3_LIBRARIES})
else()
    message(FATAL_ERROR "Libd: unsupported toolchain.")
    return()
endif()

target_include_directories(dpython
    PRIVATE
    ..
    ../..
)
target_link_libraries(dpython
    PUBLIC
    dglobals
    dutil
)
