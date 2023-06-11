#include "globals.h"

namespace DGLOBALS {

std::shared_ptr<ConcreteClassMapType> getConcreteClassMap()
{
    static auto map = std::make_shared<ConcreteClassMapType>();
    return map;
}

std::shared_ptr<InterfaceMapType> getInterfaceMap()
{
    static auto multmap = std::make_shared<InterfaceMapType>();
    return multmap;
}
} // namespace DGLOBALS
