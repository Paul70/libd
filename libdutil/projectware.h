#ifndef DUTIL_PROJECTWARE_H
#define DUTIL_PROJECTWARE_H
#include "factoryinterface.h"
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
    static std::unique_ptr<DerivedClass> createNewInstanceViaFactory(ConstructionData const &cd)
    {
        return std::make_unique<DerivedClass>(cd);
    }
};

} // namespace DUTIL

D_DECLARE_FACTORYINTERFACE(::DUTIL::ProjectWare);

#define D_DECLARE_PROJECTWARE(REGISTERED_CLASS) \
    static const DUTIL::ConcreteFactory<REGISTERED_CLASS, ::DUTIL::ProjectWare> factory;
#define D_DEFINE_PROJECTWARE(REGISTERED_CLASS) \
    const DUTIL::ConcreteFactory<REGISTERED_CLASS, ::DUTIL::ProjectWare> REGISTERED_CLASS::factory;

#endif // DUTIL_PROJECTWARE_H
