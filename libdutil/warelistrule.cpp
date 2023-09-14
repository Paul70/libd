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
    length(-1),
    key(""),
    description(""),
    type(""),
    callbackCV(WarelistRule::getTrivialConstructionValidator)
{}
