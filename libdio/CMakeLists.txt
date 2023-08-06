set(dio_HEADERS
    file.h
)

set(dio_SOURCES
    file.cpp
)

add_library(dio)
target_sources(dio PRIVATE
    ${dio_HEADERS}
    ${dio_SOURCES}
)

target_include_directories(dutil PRIVATE
    ..
    ../..
)
