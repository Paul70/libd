#include "factorytestsexampleclass.h"
#include "libd/libdutil/constructionvalidator.h"

D_DEFINE_FACTORYINTERFACE(::TESTS::Base);

using namespace TESTS;
using namespace DUTIL;

D_DEFINE_BASEINTERFACE(DerivedA)
D_DEFINE_BASEINTERFACE(DerivedB)
D_DEFINE_BASEINTERFACE(CompoundDerived)
D_DEFINE_BASEINTERFACE(TemporaryNewConcreteType)

using FI = FactoryInterface<Base>;

ConstructionValidator const &Base::getConstructionValidator()
{
    static const ConstructionValidator cv({[] {
                                              auto sr = SettingRule::forNamedParameter<BaseFlag>(SettingRule::Usage::OPTIONAL,
                                                                                                 "Base flag");
                                              sr.defaultValue = Variant();
                                              return sr;
                                          }()},
                                          {
                                              WarelistRule::forSharedWare<BaseWareAReference>("reference to const ware A"),
                                              WarelistRule::forSubobject<BaseWareAInstance>("my own ware A"),
                                          },
                                          ProjectWare::getConstructionValidator());

    return cv;
}

ConstructionValidator const &DerivedA::getConstructionValidator()
{
    static const ConstructionValidator cv({SettingRule::forNamedParameter<DerivedAInt>(SettingRule::Usage::MANDATORY_NO_DEFAULT,
                                                                                       "DerivedAInt"),
                                           SettingRule::forNamedParameter<Value>(SettingRule::Usage::OPTIONAL, "Value A")},
                                          {},
                                          Base::getConstructionValidator());

    return cv;
}

DerivedA::DerivedA(const ConstructionData &cd) :
    id(getConstructionValidator().validateNamedParameter<DerivedAInt>(cd))
{}

std::string DerivedA::identifyYourself() const
{
    return "I am DerivedA object " + Utility::toString(id);
}

ConstructionValidator const &DerivedB::getConstructionValidator()
{
    static const ConstructionValidator cv({SettingRule::forNamedParameter<DerivedBInt>(SettingRule::Usage::MANDATORY_NO_DEFAULT,
                                                                                       "DerivedBInt"),
                                           SettingRule::forNamedParameter<Value>(SettingRule::Usage::OPTIONAL, "Value B")},
                                          {},
                                          Base::getConstructionValidator());

    return cv;
}

DerivedB::DerivedB(const ConstructionData &cd) :
    id(getConstructionValidator().validateNamedParameter<DerivedBInt>(cd))
{}

void DerivedB::deconstruct(ConstructionData &cd) &&
{
    cd.setParameter<DerivedB::DerivedBInt>(id);
}

std::string DerivedB::identifyYourself() const
{
    return "I am DerivedB object " + Utility::toString(id);
}

ConstructionValidator const &CompoundDerived::getConstructionValidator()
{
    static const ConstructionValidator cv({},
                                          {WarelistRule::forSubobject<CompoundWareAInstance>("my own CompoundWareA instance")},
                                          Base::getConstructionValidator());

    return cv;
}

CompoundDerived::CompoundDerived(ConstructionData const &cd) :
    wareA(std::move(*getConstructionValidator().buildSubobject<CompoundWareAInstance>(cd)))
{}

//void CompoundDerived::deconstruct(ConstructionData & cd) &&
//{
//    cd.addSubobject<CompoundDerived::CompoundWareAInstance>(std::move(wareA));
//}

std::string CompoundDerived::identifyYourself() const
{
    return "I am a CompoundDerived object ";
}

ConstructionValidator const &TemporaryNewConcreteType::getConstructionValidator()
{
    static const ConstructionValidator cv({SettingRule::forNamedParameter<TemporaryReal>(SettingRule::Usage::OPTIONAL,
                                                                                         "Temporary real")},
                                          {},
                                          Base::getConstructionValidator());
    return cv;
}

std::string TemporaryNewConcreteType::identifyYourself() const
{
    return "";
}

TemporaryNewConcreteType::TemporaryNewConcreteType(ConstructionData const &)
{}
