#include "compoundware.h"
#include "libdutil/constructionvalidator.h"
#include "libdutil/warelistrule.h"

using namespace TESTS;
using namespace DUTIL;

D_DEFINE_PROJECTWARE(CompoundWareA);
D_DEFINE_PROJECTWARE(CompoundWareB);
D_DEFINE_PROJECTWARE(CompoundWareC);

ConstructionValidator const &CompoundWareA::getConstructionValidator()
{
    using SR = SettingRule;
    static const ConstructionValidator cv({SR::forNamedParameter<LabelA>(SR::Usage::MANDATORY_NO_DEFAULT, "a label")},
                                          {},
                                          ProjectWare::getConstructionValidator());
    return cv;
}

CompoundWareA::CompoundWareA(ConstructionData const &cd) :
    lA(getConstructionValidator().validateNamedParameter<LabelA>(cd))
{}

CompoundWareA::CompoundWareA(CompoundWareA &&other) :
    lA(other.lA)
{
    other.lA = 0; // Set to zero to make the move operation visible.
}

ConstructionValidator const &CompoundWareB::getConstructionValidator()
{
    static const ConstructionValidator
        cv({[] {
               auto sr = SettingRule::forNamedParameter<RealB>(SettingRule::Usage::MANDATORY_WITH_DEFAULT, "a real");
               sr.defaultValue = 42.1;
               return sr;
           }()},
           {WarelistRule::forSharedWare<WareAPtrRef>("pointer to const ware A")},
           ProjectWare::getConstructionValidator());
    return cv;
}

CompoundWareB::CompoundWareB(ConstructionData const &cd) :
    wareARef(getConstructionValidator().validateSharedWare<WareAPtrRef>(cd)),
    rB(getConstructionValidator().validateNamedParameter<RealB>(cd))
{}

ConstructionValidator const &CompoundWareC::getConstructionValidator()
{
    // clang-format off
    static const ConstructionValidator cv(
        {[]() {
            auto sr = SettingRule::forNamedEnum<ChoiceC>(SettingRule::Usage::OPTIONAL,"an optional choice");
            sr.defaultValue = toString(ChoiceC::THE_UGLY);
            return sr;
        }()},
        {
            WarelistRule::forSubobjectList<WareAVariableListInstance>("my own variable list of ware A"),
            WarelistRule::forSharedWareList<WareAVariableListRef>("shared variable list of ware A"),
            WarelistRule::forSubobjectList<WareAVariableListInstance>("my own fixed list of two ware A", 2),
            WarelistRule::forSharedWareList<WareAFixedListRef>("shared fixed list of two ware A", 2),
            WarelistRule::forSubobject<WareBInstance>("my own ware B object")},
        ProjectWare::getConstructionValidator());
    // clang-format on
    return cv;
}

CompoundWareC::CompoundWareC(DUTIL::ConstructionData const &cd) :
    ownWareAVariableList(getConstructionValidator().buildSubobjectList<WareAVariableListInstance>(cd)),
    sharedWareAVariableList(getConstructionValidator().validateSharedList<WareAVariableListRef>(cd)),
    ownWareAFixedList(getConstructionValidator().buildSubobjectList<WareAFixedListInstance>(cd)),
    sharedWareAFixedList(getConstructionValidator().validateSharedList<WareAFixedListRef>(cd)),
    wareB(*getConstructionValidator().buildSubObject<WareBInstance>(cd)),
    cC(getConstructionValidator().validateNamedEnum<ChoiceC>(cd))
{}
