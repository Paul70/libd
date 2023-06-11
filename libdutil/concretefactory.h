#ifndef DUTIL_CONCRETEFACTORY_H
#define DUTIL_CONCRETEFACTORY_H
#include "factory.h"
#include "factoryinterface.h"

namespace DUTIL {

template<typename ConcreteWareType, typename... Interfaces>
class ConcreteFactory : public Factory, public FactoryInterface<Interfaces>...
{
public:
    explicit ConcreteFactory()
    {
        // register concrete ware type name.
        std::string className = ConcreteWareType::getClassName();
        Factory::registerConcreteClassName(className, this);

        // register each given interface class name.
        (Factory::registerInterfaceName(FactoryInterface<Interfaces>::getInterfaceName(), className), ...);
    }

    virtual ~ConcreteFactory() override
    {
        // unregister concrete factory and associated class
        std::string className = ConcreteWareType::getClassName();
        Factory::unregisterConcreteClassName(className);

        // unregister interfaces registered with concrete class
        (Factory::unregisterInterfaceName(FactoryInterface<Interfaces>::getInterfaceName(), className), ...);
    }
};

} // end namespace DUTIL

#endif // DUTIL_CONCRETEFACTORY_H
