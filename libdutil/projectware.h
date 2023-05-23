#ifndef DUTIL_PROJECTWARE_H
#define DUTIL_PROJECTWARE_H
#include "ware.h"
#include <memory>

namespace DUTIL {
struct ConstructionData;

/*! brief description of ProjectWare
 *
 * Longer description of ProjectWare.
 */

class ProjectWare : public Ware
{
public:
    template<typename DerivedClass>
    static std::unique_ptr<DerivedClass> createNewInstanceViaFactory(ConstructionData const& cd)
    {
        return std::make_unique<DerivedClass>(cd);
    }

    void doSomething();
};

} // namespace DUTIL

#endif // DUTIL_PROJECTWARE_H
