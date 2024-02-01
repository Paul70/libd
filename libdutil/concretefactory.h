#ifndef DUTIL_CONCRETEFACTORY_H
#define DUTIL_CONCRETEFACTORY_H
#include "constructionvalidator.h"
#include "factory.h"
#include "factoryinterface.h"

namespace DUTIL {

template <typename ConcreteWareType, typename... Interfaces>
class ConcreteFactory : public Factory, public FactoryInterface<Interfaces>...
{
  public:
  explicit ConcreteFactory()
  {
    // register concrete ware type name.
    std::string className = ConcreteWareType::getClassName();
    Factory::registerConcreteClassName(className, this);

    // register each given interface class name.
    (Factory::registerInterfaceName(FactoryInterface<Interfaces>::getInterfaceName(), className),
     ...);
  }

  virtual ~ConcreteFactory() override
  {
    // unregister concrete factory and associated class
    std::string className = ConcreteWareType::getClassName();
    Factory::unregisterConcreteClassName(className);

    // unregister interfaces registered with concrete class
    (Factory::unregisterInterfaceName(FactoryInterface<Interfaces>::getInterfaceName(), className),
     ...);
  }

  //! Delete copy constructor and assignment operator since a concrete factory has to be unique.
  ConcreteFactory(ConcreteFactory const&) = delete;
  void operator=(ConcreteFactory const&) = delete;

  private:
  virtual std::string checkCDImpl(ConstructionData const& cd) const override
  {
    auto const& cv = getConcreteConstructionValidator();
    auto result = cv.check(cd);
    if (!result.empty()) {
      return "Errors occured during ConstructionData check for '"
             + ConcreteWareType::getShortClassName() + "': " + result;
    }
    return "";
  }

  virtual ConstructionValidator const& getConcreteConstructionValidatorImpl() const override
  {
    return ConcreteWareType::getConstructionValidator();
  }

  virtual std::unique_ptr<Ware> newInstanceImpl(ConstructionData const& cd) const override
  {
    // check if ConstructionData can be used to create a new ConcreteWareType object.
    auto result = Factory::checkCD(cd);
    if (!result.empty())
      D_THROW(result);

    // template keyword is required because fucntion call depends on template parameter.
    return ConcreteWareType::template createNewInstanceViaFactory<ConcreteWareType>(cd);
  }
};

}  // end namespace DUTIL

#endif  // DUTIL_CONCRETEFACTORY_H
