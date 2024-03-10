#include "zoo.h"
#include "libdutil/constructiondata.h"

using namespace TESTS;
using namespace DUTIL;

ConstructionValidator const& Zoo::getConstructionValidator()
{
  using SR = SettingRule;
  using WR = WarelistRule;
  // clang-format off
    static const ConstructionValidator cv({
        []() {
            SR sr = SR::forNamedParameter<Name>(SR::Usage::MANDATORY_NO_DEFAULT, "The name of the new zoo.");
            sr.listOfPossibleValues = {"Wilhelma","Hellabrunn","Zoologische Garten Berlin"};
            return sr;
        } (),
        []() {
            SR sr = SR::forNamedEnum<ClosingDay>(SR::Usage::MANDATORY_WITH_DEFAULT, "The zoo's rest day.");
            sr.defaultValue = Variant(Zoo::ClosingDay::MONDAY);
            return sr;
        }(),
        []() {
            SR sr = SR::forNamedParameter<Min_Visitors>(SR::Usage::MANDATORY_WITH_DEFAULT, "The minimal number of visitors needed to open the zoo.");
            sr.minimalValue = Variant(10);
            sr.maximalValue = Variant(50);
            sr.defaultValue = Variant(20);
            return sr;
        }(),
        []() {
            SR sr = SR::forNamedParameter<Max_Visitors>(SR::Usage::MANDATORY_WITH_DEFAULT, "The maximal number of visitors before the zoo closes.");
            sr.maximalValue = Variant(10000);
            sr.minimalValue = Variant(1000);
            sr.defaultValue = Variant(8000);
            return sr;
        }()
    }, // end list of setting rules
    { [](){
        WR wr = WR::forSubobjectList<CatList>("A subobject list for cats living in the zoo.");
        wr.usage = WR::Usage::OPTIONAL;
        return wr;}(),
      [](){
        WR wr = WR::forSubobject<SingleTiger>("A subobject for a tiger living in the zoo.");
        wr.usage = WR::Usage::OPTIONAL;
        return wr;}()
    } // end list of warelist rules
    //ProjectWare::getConstructionValidator()
    );
  // clang-format on
  return cv;
}

Zoo::Zoo(ConstructionData const& cd) :
    name_(getConstructionValidator().validateNamedParameter<Name>(cd)),
    off_(getConstructionValidator().validateNamedEnum<Zoo::ClosingDay>(cd)),
    min_(getConstructionValidator().validateNamedParameter<Min_Visitors>(cd)),
    max_(getConstructionValidator().validateNamedParameter<Max_Visitors>(cd))
{
  auto catlist = getConstructionValidator().buildSubobjectList<CatList>(cd);
  for (auto& cat : catlist) {
    catmap_.emplace(cat->getName().value(), std::move(cat));
  }

  auto singleCat = getConstructionValidator().buildSubobject<SingleTiger>(cd);
  if (singleCat) {
    catmap_.emplace(singleCat->getName().value(), std::move(singleCat));
  }
}

bool Zoo::findCat(std::string name) const
{
  if (catmap_.find(name) != catmap_.cend())
    return true;
  else
    return false;
}

Zoo::Name Zoo::getName() const
{
  return name_;
}

Zoo::Min_Visitors Zoo::getMinVisitors() const
{
  return min_;
}

Zoo::Max_Visitors Zoo::getMaxVisitors() const
{
  return max_;
}

Zoo::ClosingDay Zoo::getOffDay() const
{
  return off_;
}
