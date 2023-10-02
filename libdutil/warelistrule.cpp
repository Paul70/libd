#include "warelistrule.h"
#include "constructionvalidator.h"

using namespace DUTIL;

ConstructionValidator const &WarelistRule::getTrivialConstructionValidator()
{
    static ConstructionValidator cv;
    return cv;
}

WarelistRule::WarelistRule() :
    usage(Usage::MANDATORY),
    asReference(false),
    length(lengthNotDefined),
    key(""),
    description(""),
    type(""),
    callbackCV(WarelistRule::getTrivialConstructionValidator)
{}
