set(dglobals_HEADERS
    globals.h
)

set(dglobals_SOURCES
    globals.cpp
)

# Building dglobals as a shrared library explicitly is necessary since
# it contains global variables.
add_library(dglobals SHARED)
target_sources(dglobals PRIVATE
    ${dglobals_HEADERS}
    ${dglobals_SOURCES}
)
target_include_directories(dglobals PUBLIC ../..)

