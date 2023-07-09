#include "ware.h"
#include "exception.h"
#include "namedclass.h"

using namespace DUTIL;

std::string Ware::getConcreteClassName() const
{
    NamedClass const *nc = dynamic_cast<NamedClass const *>(this);
    D_ASSERT(nc);
    return nc->getClassName();
}

std::string Ware::getShortConcreteClassName() const
{
    NamedClass const *nc = dynamic_cast<NamedClass const *>(this);
    D_ASSERT(nc);
    return nc->getShortClassName();
}
