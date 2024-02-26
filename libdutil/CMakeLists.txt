set(dutil_HEADERS
    basictypes.h
    clock.h
    concretefactory.h
    constructiondata.h
    constructionvalidator.h
    dataset.h
    datasetrule.h
    exception.h
    factory.h
    factoryinterface.h
    logitem.h
    loggingsource.h
    loggingsink.h
    namedclass.h
    namedenum.h
    namedparameter.h
    namedreference.h
    namedreferenceparameter.h
    now.h
    overload.h
    projectware.h
    settingrule.h
    settings.h
    staticpointercast.h
    streamloggingsink.h
    ticker.h
    time.h
    types.h
    utility.h
    variant.h
    ware.h
    warelistrule.h
)

set(dutil_SOURCES
    basictypes.cpp
    clock.cpp
    constructiondata.cpp
    constructionvalidator.cpp
    dataset.cpp
    datasetrule.cpp
    exception.cpp
    factory.cpp
    factoryinterface.cpp
    loggingsink.cpp
    loggingsource.cpp
    namedenum.cpp
    namedreferenceparameter.cpp
    now.cpp
    projectware.cpp
    settingrule.cpp
    settings.cpp
    streamloggingsink.cpp
    ticker.cpp
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



