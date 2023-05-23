#ifndef DUTIL_WARE_H
#define DUTIL_WARE_H
#include "namedparameter.h"
#include <string>

namespace DUTIL {

class Ware
{
public:
    D_NAMED_STRING(Type);

    std::string getConcreteClassName() const;

    std::string getShortConcreteClassName() const;

    virtual ~Ware() = default;
};

} // namespace DUTIL

#endif // DUTIL_WARE_H
