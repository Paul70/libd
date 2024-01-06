#ifndef DUTIL_FACTORY_H
#define DUTIL_FACTORY_H
#include <memory>
#include <set>
#include <string>
#include "basictypes.h"

namespace DUTIL {
using namespace BasicTypes;
class Ware;
struct ConstructionData;
class ConstructionValidator;

class Factory
{
  public:
  virtual ~Factory() = default;

  /*! \brief Check construction data for the concrete class.
     *
     * The return string contains an error message.
     * If everything is ok, this string is empty. If not, an exception is thrown containing the error message.
     */
  std::string checkCD(ConstructionData const& cd) const;

  /*! \brief Return a reference to a satic ConstructionValidator object.
     *
     * The ConstructionValidator object is especially configuerd to check the concrete ware.
     * Therefore, the method is non static.
     */
  ConstructionValidator const& getConstructionValidator() const;

  std::unique_ptr<Ware> newInstance(ConstructionData const& cd) const;

  /*! \brief Get all currently registered concrete project class names.
     *
     * The list contains all class names which have an associated factory object.
     * If no class names are registered, an empty list will be returned.
     */
  static StringList getConcreteClasses() noexcept;

  //! Return a std::set containing all diffrent, registered interface names.
  static std::set<std::string> getInterfaces() noexcept;

  //! Get a list of all concrete class types registered for a specific interface name.
  static StringList getConcreteClassesForInterface(const std::string interfaceName) noexcept;

  //! Check if the specified class name is registered as a concrete factroy product.
  static bool isRegisteredConcreteClassName(const std::string className) noexcept;

  //! Check if the specified interface name is registered as an interface class for a concrete class type.
  static bool isRegisteredInterface(const std::string interfaceName) noexcept;

  //! Check if the combination "interfaceName - concreteClassName" is a valid entry in the interface map.
  static bool isRegisteredWithInterface(const std::string interfaceName,
                                        const std::string concreteClassName);

  //! Return an (abstract) Factory class reference for the given, concrete class name.
  static Factory const& getFactoryByName(std::string const className);

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
  static void registerConcreteClassName(const std::string className, Factory* me);

  /*! \brief Remove a class name and its associtated factory from the global factory map.
     *
     * Function is calles during deconstruction of a concrete factory object and
     * erases the porject class name and factory pair from the global map.
     *
     * Function asserts if the given class name is not found in the map.
     */
  static void unregisterConcreteClassName(const std::string className);

  static void registerInterfaceName(const std::string interfaceName,
                                    const std::string concreteClassName);

  static void unregisterInterfaceName(const std::string interfaceName,
                                      const std::string concreteClassName);

  private:
  virtual ConstructionValidator const& getConstructionValidatorImpl() const = 0;
  virtual std::string checkCDImpl(ConstructionData const& cd) const = 0;
  virtual std::unique_ptr<Ware> newInstanceImpl(ConstructionData const& cd) const = 0;
  std::shared_ptr<void> factoryMap_;
  std::shared_ptr<void> interfaceMap_;
};

}  // end namespace DUTIL
#endif  // DUTIL_FACTORY_H
