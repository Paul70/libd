#include "cat.h"
#include "libdutil/constructiondata.h"
#include "libdutil/constructionvalidator.h"
#include "libdutil/settingrule.h"

using namespace TESTS;
using namespace DUTIL;

D_DEFINE_FACTORYINTERFACE(::TESTS::Cat)

ConstructionValidator const& Cat::getConstructionValidator()
{
  using SR = SettingRule;
  // clang-format off
    static const ConstructionValidator cv({
        [](){
          return SR::forNamedParameter<Cat::Name>(SR::Usage::MANDATORY_NO_DEFAULT, "The unique name of this tiger.");
        }(),
        [](){
          return SR::forNamedParameter<Cat::Age>(SR::Usage::MANDATORY_NO_DEFAULT, "The current age of this tiger.");
        }()
        },
        {}, // empty warelist rules map
        ProjectWare::getConstructionValidator());
  // clang-format on
  return cv;
}

Cat::Cat(ConstructionData const& cd) :
    name_(getConstructionValidator().validateNamedParameter<Name>(cd)),
    age_(getConstructionValidator().validateNamedParameter<Age>(cd))
{}

Cat::Cat() :
    name_("no_name"),
    age_(0)
{}

Cat::Type Cat::getType() const
{
  return getTypeImpl();
}

Cat::Name Cat::getName() const
{
  return name_;
}

Cat::Age Cat::getAge() const
{
  return age_;
}

D_DEFINE_CAT(Tiger)

ConstructionValidator const& Tiger::getConstructionValidator()
{
  using SR = SettingRule;
  // clang-format off
        static ConstructionValidator cv({
            [](){
                SR sr = SR::forNamedEnum<Tiger::Species>(SR::Usage::MANDATORY_WITH_DEFAULT, "The species of this tiger.");
                sr.defaultValue = Variant(Tiger::Species::AMUR);
                return sr;
            }()
            },
            {}, // empty warelist rules map
            Cat::getConstructionValidator());
  // clang-format on
  return cv;
}

Tiger::Tiger(ConstructionData const& cd) :
    Cat(cd),
    species_(getConstructionValidator().validateNamedEnum<Tiger::Species>(cd))
{}

D_DEFINE_CAT(Jaguar)

ConstructionValidator const& Jaguar::getConstructionValidator()
{
  // clang-format off
        static ConstructionValidator cv(
            {}, // emtpy settingrule map
            {}, // empty warelistrule map
            Cat::getConstructionValidator());
  // clang-format on
  return cv;
}

Jaguar::Jaguar(ConstructionData const& cd) :
    Cat(cd)
{}

D_DEFINE_CAT(KK)
