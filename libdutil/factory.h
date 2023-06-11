#ifndef DUTIL_FACTORY_H
#define DUTIL_FACTORY_H
#include "basictypes.h"
#include "exception.h"
#include <memory>
#include <set>
#include <string>

namespace DUTIL {

class Factory
{
public:
    virtual ~Factory() = default;

    /*! \brief Get all currently registered concrete project class names.
     *
     * The list contains all class names which have an associated factory object.
     * If no class names are registered, an empty list will be returned.
     */
    static StringList getConcreteClasses() noexcept;

    static std::set<std::string> getInterfaces() noexcept;

    static StringList getConcreteClassesForInterface(const std::string interfaceName) noexcept;

    static bool isRegisteredConcreteClassName(const std::string className) noexcept;

    static bool isRegisteredInterface(const std::string interfaceName) noexcept;

    static bool isRegisteredWithInterface(const std::string interfaceName, const std::string concreteClassName);

    static Factory const &getFactoryByName(std::string const className);

protected:
    /*! \brief Register a new concrete factory.
     *
     * Function adds a new project class name and its associateted concrete factory to the
     * global map of factories. It also sets the factoryMap_ and interfaceMap_ member pointer to ensure
     * that the lifetime of the factory and the concrete class object are mateched.
     * Functions gets called during construction of a (static) concrete factory object.
     *
     * Functions asserts in case the given class name is already registered with a factory.
     */
    static void registerConcreteClassName(const std::string className, Factory *me);

    /*! \brief Remove a class name and its associtated factory from the global factory map.
     *
     * Function is calles during deconstruction of a concrete factory object and
     * erases the porject class name and factory pair from the global map.
     *
     * Function asserts if the given class name is not found in the map.
     */
    static void unregisterConcreteClassName(const std::string className);

    static void registerInterfaceName(const std::string interfaceName, const std::string concreteClassName);

    static void unregisterInterfaceName(const std::string interfaceName, const std::string concreteClassName);

private:
    std::shared_ptr<void> factoryMap_;
    std::shared_ptr<void> interfaceMap_;
};

} // end namespace DUTIL
#endif // DUTIL_FACTORY_H
