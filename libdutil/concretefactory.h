#ifndef DUTIL_CONCRETEFACTORY_H
#define DUTIL_CONCRETEFACTORY_H
#include "factory.h"

namespace DUTIL {

template<typename ConcreteWareType, typename... Interfaces>
class ConcreteFactory : public Factory
{
public:
    ConcreteFactory()
    {
        // hier rein kommst register class call und eine fold expression um die inerfaces zu setzen
    }
};

} // end namespace DUTIL

#endif // DUTIL_CONCRETEFACTORY_H
