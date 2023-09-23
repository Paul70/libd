#ifndef DUTIL_WARELISTRULE_H
#define DUTIL_WARELISTRULE_H
#include "basictypes.h"
#include "namedreference.h"
#include <functional>

namespace DUTIL {
class ConstructionValidator;

/*! \brief WarelistRule: A structure holding parameters to characterize subobject
 *  ConstructionData inside a top level ConstructionData object.
 *
 * The parameters are:
 * - length (Type label_t aka int): defines the number of subobject CDs (within a superior CD object) which all
 *   refer to the same type. If length is set to not defined (-1) that means the number of subobject cds is not known a priori.
 * - key (std::string): string key do identify the warelist rule within a map.
 * - description (std::string): a descriptive string for the given warelist rule.
 * - type (std::string): the string name of the type the warelist rule refers to.
 * - callbackCV (std::fucntion): a fucntor containing the "getConstructionValidator()" method of the referred type.
 */

struct WarelistRule
{
    static constexpr label_t lengthNotDefined = -1;
    static constexpr label_t lengthSingleton = 1;

    static ConstructionValidator const &getTrivialConstructionValidator();

    D_NAMED_ENUM(Usage, MANDATORY, OPTIONAL)
    Usage usage;

    label_t length;
    std::string key;
    std::string description;
    std::string type;
    std::function<ConstructionValidator const &()> callbackCV;

    //! Default initialization
    WarelistRule();

    template<typename NR>
    static WarelistRule forSubobjectList(std::string d, label_t length = lengthNotDefined)
    {
        WarelistRule rule;
        rule.length = length;
        rule.description = d;
        rule.key = NR::getReferenceName();
        rule.type = NR::getReferredTypeName();
        rule.callbackCV = NR::RT::getConstructionValidator;
        return rule;
    }

    template<typename NR>
    static WarelistRule forSubobject(std::string d)
    {
        return WarelistRule::forSubobjectList<NR>(d, lengthSingleton);
    }
};

} // namespace DUTIL
#endif // DUTIL_WARELISTRULE_H
