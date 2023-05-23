set(dutil_HEADERS
    basictypes.h
    concretefactory.h
    constructiondata.h
    exception.h
    factory.h
    factoryinterface.h
    namedclass.h
    namedenum.h
    namedparameter.h
    projectware.h
    settings.h
    utility.h
    variant.h
    ware.h
)

set(dutil_SOURCES
    basictypes.cpp
    constructiondata.cpp
    exception.cpp
    factory.cpp
    factoryinterface.cpp
    namedenum.cpp
    projectware.cpp
    settings.cpp
    utility.cpp
    variant.cpp
    ware.cpp
)

add_library(dutil)
target_sources(dutil PRIVATE
    ${dutil_HEADERS}
    ${dutil_SOURCES}
)

target_include_directories(dutil PRIVATE .. ../.. )
target_link_libraries(dutil PUBLIC dglobals)
