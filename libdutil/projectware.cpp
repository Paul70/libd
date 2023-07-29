#include "projectware.h"
#include "constructionvalidator.h"
#include <iostream>
using namespace DUTIL;

D_DEFINE_FACTORYINTERFACE(::DUTIL::ProjectWare);

ConstructionValidator const &ProjectWare::getConstructionValidator()
{
    using SR = SettingRule;
    // clang-format off
    static ConstructionValidator cv = ConstructionValidator(
        {[] () {
            return SR::forNamedParameter<Ware::Type>(SR::Usage::OPTIONAL, "Set the type of the object these construction data are meant for.");
            } ()
        });
    // clang-format on
    return cv;
}
