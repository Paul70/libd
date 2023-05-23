set(dglobals_HEADERS
    globals.h
)

set(dglobals_SOURCES
    globals.cpp
)

add_library(dglobals SHARED)
target_sources(dglobals PRIVATE
    ${dglobals_HEADERS}
    ${dglobals_SOURCES}
)
