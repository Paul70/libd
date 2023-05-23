#include "ware.h"
#include "namedclass.h"

using namespace DUTIL;

std::string Ware::getConcreteClassName() const
{
    NamedClass const* nc = dynamic_cast<NamedClass const*>(this);
    // hier noch in assert rein falls das nicht geklappt hat und nc ein nullptr ist
    return nc->getClassName();
}

std::string Ware::getShortConcreteClassName() const
{
    NamedClass const* nc = dynamic_cast<NamedClass const*>(this);
    // hier noch ein assert rein falls das nicht geklappt hat und nc ein nullptr ist
    return nc->getShortClassName();
}
