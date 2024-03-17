set(libd_tests_HEADERS
    cat.h
    compoundware.h
    factorytestsexampleclass.h
    testbase.h
    testdummy.h
    testdummyinterface.h
    trivialware.h
    zoo.h
)
set(libd_tests_SOURCES
    cat.cpp
    compoundware.cpp
    factorytestsexampleclass.cpp
    main.cpp
    testbase.cpp
    testdummy.cpp
    testdummyinterface.cpp
    trivialware.cpp
    zoo.cpp
    libdutil/basictypestests.cpp
    libdutil/clocktests.cpp
    libdutil/constructiondatatests.cpp
    libdutil/constructionvalidatortests.cpp
    libdutil/datasettests.cpp
    libdutil/factoryinterfacetests.cpp
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
    libdversion/versiontests.cpp
)

find_package(GTest CONFIG REQUIRED)

add_executable(libd_tests)
target_sources(libd_tests PRIVATE
    ${libd_tests_HEADERS}
    ${libd_tests_SOURCES}
)

target_include_directories(libd_tests PUBLIC
    ..
    ../..
)
target_link_libraries(libd_tests PUBLIC
    GTest::gtest
    GTest::gmock
    dglobals
    dutil
    dversion
)

