#include "settingrule.h"

using namespace DUTIL;

SettingRule::SettingRule() :
    usage(Usage::OPTIONAL),
    key(""),
    description(""),
    listOfPossibleValues({}),
    type(Variant::Type::STRING),
    defaultValue(),
    minimalValue(),
    maximalValue(),
    minimalStringLength(0)
{}
