set(libd_tests_HEADERS
    testbase.h
    testdummy.h
)
set(libd_tests_SOURCES
    main.cpp
    testbase.cpp
    testdummy.cpp
    libdutil/basictypestests.cpp
    libdutil/factorytests.cpp
    libdutil/namedclasstests.cpp
    libdutil/namedenumtests.cpp
    libdutil/namedparametertests.cpp
    libdutil/utilitytests.cpp
    libdutil/varianttests.cpp
    libdutil/waretests.cpp
)

add_executable(libd_tests)
target_sources(libd_tests PRIVATE
    ${libd_tests_HEADERS}
    ${libd_tests_SOURCES}
)

target_include_directories(libd_tests PUBLIC
    ..
    ../..
)

# Internal library dependencies
target_link_libraries(libd_tests PUBLIC
    dglobals
    dutil
)

# External library dependencies
target_link_libraries(libd_tests PUBLIC
    gtest
    gmock
    pthread
)


