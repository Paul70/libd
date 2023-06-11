#ifndef DUTIL_WARE_H
#define DUTIL_WARE_H
#include "namedparameter.h"
#include <string>

namespace DUTIL {

/*! \brief Interface class for objects that can be build using a concrete factroy.
 *
 * short explanation why we do this.
 */
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
