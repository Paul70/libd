#include "libdutil/constructiondata.h"
#include "libdutil/constructionvalidator.h"
#include "libdutil/namedclass.h"
#include "libdutil/projectware.h"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class ConstructionValidatorTests : public TestBase
{};

class Zoo : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::Zoo)
{
public:
    D_NAMED_ENUM(Cat, TIGER, LION, JAGUAR, LUX)

    static DUTIL::ConstructionValidator const &getConstructionValidator()
    {
        using SR = SettingRule;
        static const ConstructionValidator cv({[]() {
            SR sr = SR::forNamedEnum<Cat>(SR::Usage::MANDATORY_WITH_DEFAULT, "The cat living in the zoo");
            sr.defaultValue = Variant(Cat::LION);
            return sr;
        }()});
        return cv;
    }

    explicit Zoo(DUTIL::ConstructionData const &cd) :
        cat_(cd.s.getEnum<Cat>())
    {}

private:
    Cat cat_;
};

} // namespace

TEST_F(ConstructionValidatorTests, testSomethingForValueEquality)
{
    // get ConstructionData for a Zoo object
    ConstructionData cd = ConstructionData().setEnum<Zoo::Cat>(Zoo::Cat::JAGUAR);
    auto cv = Zoo::getConstructionValidator();
    if (cv.check(cd).empty())
        Zoo z = Zoo(cd);
}

TEST_F(ConstructionValidatorTests, testSomethingForException)
{
    //W_EXPECT_THROW("do something exceptional", "something bad is going on");
}
