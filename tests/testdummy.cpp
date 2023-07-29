#include "testdummy.h"
#include "libd/libdutil/constructiondata.h"
#include "libd/libdutil/constructionvalidator.h"

using namespace LIBD;
using namespace TESTS;
using namespace DUTIL;

D_DEFINE_PROJECTWARE(TestDummy);

DUTIL::ConstructionValidator const &TestDummy::getConstructionValidator()
{
    using SR = SettingRule;
    // clang-format off
    static ConstructionValidator cv = ConstructionValidator(
        { [] () {
            SR sr = SR::forNamedEnum<COLOR>(SR::Usage::MANDATORY_WITH_DEFAULT, "Set the color member for this class.");
            sr.defaultValue = Variant(COLOR::BLUE);
            return sr;} ()
        },
        ProjectWare::getConstructionValidator());
    // clang-format on
    return cv;
}

TestDummy::TestDummy(DUTIL::ConstructionData const &cd) :
    color_(getConstructionValidator().validateNamedEnum<COLOR>(cd))
{}

TestDummy::COLOR const &TestDummy::getNamedEnum() const
{
    return color_;
}
