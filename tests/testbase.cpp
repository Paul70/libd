#include "testbase.h"
#include "libd/libdglobals/globals.h"

std::size_t TestBase::getNumberOfRegisteredConcreteClasses() noexcept
{
    // concreteClassMap is a std::map, that is we have one to one mapping.
    auto map = DGLOBALS::getConcreteClassMap();
    return map->size();
}

std::size_t TestBase::getNumberOfRegisteredInterfaces() noexcept
{
    // concreteClassMap is a std::multimap, that is we may have many to one mapping.
    auto map = DGLOBALS::getInterfaceMap();
    size_t count = 1;
    DGLOBALS::InterfaceMapType::const_iterator it = map->cbegin(), next = it;
    ++next;
    for (; next != map->cend(); ++next) {
        if ((it++)->first != (next)->first) {
            ++count;
        }
    }
    return count;
}

std::size_t TestBase::getNumberOfConcreteClassesForInterface(std::string const &interface) noexcept
{
    auto range = DGLOBALS::getInterfaceMap()->equal_range(interface);
    return std::distance(range.first, range.second);
}
