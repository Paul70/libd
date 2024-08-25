find_package(GTest CONFIG REQUIRED)
add_subdirectory(libtesting)

set(libd_tests_HEADERS
    testbase.h
)
set(libd_tests_SOURCES
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
    main.cpp
    testbase.cpp
)


add_executable(libd_tests)
target_sources(libd_tests PRIVATE
    ${libd_tests_HEADERS}
    ${libd_tests_SOURCES}
)

target_include_directories(libd_tests PRIVATE
    ..
    ../..
)
target_link_libraries(libd_tests PRIVATE
    GTest::gtest
    GTest::gmock
    dglobals
    dutil
    dversion
    testing
)

