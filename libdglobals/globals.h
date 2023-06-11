#ifndef DGLOBALS_GLOBALS_H
#define DGLOBALS_GLOBALS_H
#include <map>
#include <memory>
#include <string>

namespace DUTIL {
class Factory;
} // namespace DUTIL

namespace DGLOBALS {

//! Define the type of map used for associating concrete class names (given as std::string)
//! with their corresponding Factory object (given as a pointer).
using ConcreteClassMapType = std::map<std::string, DUTIL::Factory *>;

//! Return a shared pointer to the global, mutable factory map.
std::shared_ptr<ConcreteClassMapType> getConcreteClassMap();

//! Define the type of map for associating concrete, derived classes
//! with their corresponding interfaces.
//! The first string is the interface name and the second string the concrete class name.
using InterfaceMapType = std::multimap<std::string, std::string>;

//! Return a shared pointer to the mutable interface multimap.
std::shared_ptr<InterfaceMapType> getInterfaceMap();

} // namespace DGLOBALS

#endif // DGLOBALS_GLOBALS_H
