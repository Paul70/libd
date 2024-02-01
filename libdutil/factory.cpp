#include "factory.h"
#include "constructiondata.h"
#include "exception.h"
#include "libd/libdglobals/globals.h"
#include "ware.h"

namespace DUTIL {

std::string Factory::checkCD(ConstructionData const& cd) const
{
  return checkCDImpl(cd);
}

ConstructionValidator const& Factory::getConcreteConstructionValidator() const
{
  return getConcreteConstructionValidatorImpl();
}

std::unique_ptr<Ware> Factory::newInstance(ConstructionData const& cd) const
{
  return newInstanceImpl(cd);
}

StringList Factory::getConcreteClasses() noexcept
{
  auto const map = DGLOBALS::getConcreteClassMap();
  StringList list{};
  list.reserve(map->size());
  for (const auto& element : *map) {
    list.emplace_back(element.first);
  }
  return list;
}

std::set<std::string> Factory::getInterfaces() noexcept
{
  std::set<std::string> setIF;
  auto const map = DGLOBALS::getInterfaceMap();
  for (auto const& interface : *map.get()) {
    setIF.insert(interface.first);
  }
  return setIF;
}

StringList Factory::getConcreteClassesForInterface(const std::string interfaceName) noexcept
{
  auto range = DGLOBALS::getInterfaceMap()->equal_range(interfaceName);
  StringList list;
  list.reserve(std::distance(range.first, range.second));
  for (auto it = range.first; it != range.second; ++it) {
    list.emplace_back(it->second);
  }
  return list;
}

bool Factory::isRegisteredConcreteClassName(const std::string className) noexcept
{
  auto const map = DGLOBALS::getConcreteClassMap();
  return (map->find(className) != map->end());
}

bool Factory::isRegisteredInterface(const std::string interfaceName) noexcept
{
  auto const map = DGLOBALS::getInterfaceMap();
  return (map->find(interfaceName) != map->end());
}

bool Factory::isRegisteredWithInterface(const std::string interfaceName,
                                        const std::string concreteClassName)
{
  auto const map = DGLOBALS::getInterfaceMap();
  auto range = map->equal_range(interfaceName);
  for (auto it = range.first; it != range.second; ++it) {
    if (it->second == concreteClassName) {
      return true;
    }
  }
  return false;
}

Factory const& Factory::getFactoryByName(std::string const className)
{
  D_ASSERT(isRegisteredConcreteClassName(className));
  auto const map = DGLOBALS::getConcreteClassMap();
  return *map->find(className)->second;
}

void Factory::registerConcreteClassName(const std::string className, Factory* me)
{
  D_ASSERT(!isRegisteredConcreteClassName(className));
  auto map = DGLOBALS::getConcreteClassMap();
  map->emplace(className, me);

  // Embedding the global factory maps in the concrete factories
  // guarantees, that the global maps live at least as long as the concrete
  // factories. This is especially important at program finalization so that the
  // global maps does not get cleaned up before the concrete factory objects.
  me->factoryMap_ = map;
  me->interfaceMap_ = DGLOBALS::getInterfaceMap();
}

void Factory::unregisterConcreteClassName(const std::string className)
{
  D_ASSERT(isRegisteredConcreteClassName(className));
  auto map = DGLOBALS::getConcreteClassMap();
  map->erase(className);
}

void Factory::registerInterfaceName(const std::string interfaceName,
                                    const std::string concreteClassName)
{
  D_ASSERT(!isRegisteredWithInterface(interfaceName, concreteClassName));
  auto map = DGLOBALS::getInterfaceMap();
  map->emplace(interfaceName, concreteClassName);
}

void Factory::unregisterInterfaceName(const std::string interfaceName,
                                      const std::string concreteClassName)
{
  D_ASSERT(isRegisteredWithInterface(interfaceName, concreteClassName));
  auto map = DGLOBALS::getInterfaceMap();
  auto range = map->equal_range(interfaceName);
  auto it = range.first;
  while (it != range.second && it != map->end()) {
    (it->second == concreteClassName) ? map->erase(it++) : ++it;
  }
}
}  // namespace DUTIL
