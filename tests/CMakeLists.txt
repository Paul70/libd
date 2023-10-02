set(libd_tests_HEADERS
    cat.h
    compoundware.h
    testbase.h
    testdummy.h
    testdummyinterface.h
    zoo.h
)
set(libd_tests_SOURCES
    cat.cpp
    compoundware.cpp
    main.cpp
    testbase.cpp
    testdummy.cpp
    testdummyinterface.cpp
    zoo.cpp
    libdutil/basictypestests.cpp
    libdutil/constructiondatatests.cpp
    libdutil/constructionvalidatortests.cpp
    libdutil/factorytests.cpp
    libdutil/loggingtests.cpp
    libdutil/namedclasstests.cpp
    libdutil/namedenumtests.cpp
    libdutil/namedparametertests.cpp
    libdutil/namedreferencetests.cpp
    libdutil/settingruletests.cpp
    libdutil/settingstests.cpp
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


