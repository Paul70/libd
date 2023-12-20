set(dutil_HEADERS
    basictypes.h
    concretefactory.h
    constructiondata.h
    constructionvalidator.h
    dataset.h
    exception.h
    factory.h
    factoryinterface.h
    fileloggingsink.h
    logitem.h
    loggingsource.h
    loggingsink.h
    namedclass.h
    namedenum.h
    namedparameter.h
    namedreference.h
    projectware.h
    settingrule.h
    settings.h
    staticpointercast.h
    streamloggingsink.h
    time.h
    types.h
    utility.h
    variant.h
    ware.h
    warelistrule.h
)

set(dutil_SOURCES
    basictypes.cpp
    constructiondata.cpp
    constructionvalidator.cpp
    dataset.cpp
    exception.cpp
    factory.cpp
    factoryinterface.cpp
    fileloggingsink.cpp
    loggingsink.cpp
    loggingsource.cpp
    namedenum.cpp
    projectware.cpp
    settingrule.cpp
    settings.cpp
    streamloggingsink.cpp
    utility.cpp
    variant.cpp
    ware.cpp
    warelistrule.cpp
)

add_library(dutil SHARED)
target_sources(dutil PRIVATE
    ${dutil_HEADERS}
    ${dutil_SOURCES}
)

target_include_directories(dutil PRIVATE
    ..
    ../..
)
target_link_libraries(dutil PUBLIC dglobals)


