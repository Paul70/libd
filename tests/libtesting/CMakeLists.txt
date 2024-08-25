set(testing_HEADERS
    compoundware.h
    factorytestsexampleclass.h
    testdummy.h
    testdummyinterface.h
    trivialware.h
    zoo/cat.h
    zoo/zoo.h
)

set(testing_SOURCES
    compoundware.cpp
    factorytestsexampleclass.cpp
    testdummy.cpp
    testdummyinterface.cpp
    trivialware.cpp
    zoo/cat.cpp
    zoo/zoo.cpp
)

add_library(testing SHARED)
target_sources(testing
    PRIVATE
    ${testing_HEADERS}
    ${testing_SOURCES}
)
target_include_directories(testing PRIVATE
    ${D_PROJECT_DIR}/libdutil
    ${D_PROJECT_DIR}/tests
    ${D_PROJECT_DIR}/tests/libtesting
)
target_link_libraries(testing
    PRIVATE
    dglobals
    dutil
)
