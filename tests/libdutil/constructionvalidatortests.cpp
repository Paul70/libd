#include "libdutil/constructiondata.h"
#include "libdutil/constructionvalidator.h"
#include "libdutil/namedclass.h"
#include "libdutil/projectware.h"
#include "libdutil/settingrule.h"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class ConstructionValidatorTests : public TestBase
{};

class Zoo : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::Zoo)
{
public:
    D_NAMED_STRING(Name)

    D_NAMED_LABEL(Min_Visitors)
    D_NAMED_LABEL(Max_Visitors)

    D_NAMED_ENUM(Cat, TIGER, LION, JAGUAR, LUX)

    static DUTIL::ConstructionValidator const &getConstructionValidator()
    {
        using SR = SettingRule;
        // clang-format off
        static const ConstructionValidator cv({
            []() {
                SR sr = SR::forNamedParameter<Name>(SR::Usage::MANDATORY_NO_DEFAULT, "The name of the new zoo.");
                sr.listOfPossibleValues = {"Wilhelma","Hellabrunn","Zoologische Garten Berlin"};
                return sr;
            } (),
            []() {
                SR sr = SR::forNamedEnum<Cat>(SR::Usage::MANDATORY_WITH_DEFAULT, "The type of wildcat living in the zoo.");
                sr.defaultValue = Variant(Cat::LION);
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
        });
        // clang-format on
        return cv;
    }

    explicit Zoo(DUTIL::ConstructionData const &cd) :
        cat_(getConstructionValidator().validateNamedEnum<Cat>(cd)),
        name_(getConstructionValidator().validateNamedParameter<Name>(cd)),
        min_(getConstructionValidator().validateNamedParameter<Min_Visitors>(cd)),
        max_(getConstructionValidator().validateNamedParameter<Max_Visitors>(cd))
    {}

    Cat getCat() const
    {
        return cat_;
    }

    Name getName() const
    {
        return name_;
    }

    Min_Visitors getMinVisitors() const
    {
        return min_;
    }

    Max_Visitors getMaxVisitors() const
    {
        return max_;
    }

private:
    Cat cat_;
    Name name_;
    Min_Visitors min_;
    Max_Visitors max_;
};

} // namespace

TEST_F(ConstructionValidatorTests, testDefaultConstruction)
{
    ConstructionData cd;
    ConstructionValidator cv = ConstructionValidator();
    ASSERT_TRUE(cv.getListOfSettingRuleKeys().empty());
    ASSERT_TRUE(cv.check(cd).empty());
}

TEST_F(ConstructionValidatorTests, testConstructionWithSettingRulesAndValidation)
{
    {
        ConstructionData cd = ConstructionData().setEnum<Zoo::Cat>(Zoo::Cat::JAGUAR);
        cd.s.setParameter<Zoo::Name>("Hellabrunn");
        auto cv = Zoo::getConstructionValidator();
        auto validationError = cv.check(cd);
        ASSERT_TRUE(validationError.empty());
        Zoo z = Zoo(cd);
        ASSERT_TRUE(z.getCat() == Zoo::Cat::JAGUAR);
    }
    {
        // zoo name not in list of possible values
        ConstructionData cd = ConstructionData().setEnum<Zoo::Cat>(Zoo::Cat::JAGUAR);
        cd.s.setParameter<Zoo::Name>("Paul's Zoo");
        auto cv = Zoo::getConstructionValidator();
        auto validationError = cv.check(cd);
        ASSERT_FALSE(validationError.empty());
    }

    // test construction using the default value for Cat memeber
    {
        ConstructionData cd = ConstructionData().setParamter<Zoo::Name>("Wilhelma");
        auto cv = Zoo::getConstructionValidator();
        auto validationError = cv.check(cd);
        ASSERT_TRUE(validationError.empty());
        Zoo z = Zoo(cd);
        ASSERT_TRUE(z.getCat() == Zoo::Cat::LION);
    }
    {
        // min and max visitor numbers within the limits
        Zoo z(ConstructionData()
                  .setEnum<Zoo::Cat>(Zoo::Cat::TIGER)
                  .setParamter<Zoo::Name>("Hellabrunn")
                  .setParamter<Zoo::Min_Visitors>(50)
                  .setParamter<Zoo::Max_Visitors>(1000));
        ASSERT_TRUE(z.getName().value() == "Hellabrunn");
        ASSERT_TRUE(z.getCat() == Zoo::Cat::TIGER);
        ASSERT_TRUE(z.getMinVisitors() == 50);
        ASSERT_TRUE(z.getMaxVisitors() == 1000);
    }
    {
        // min and max visitor numbers within the limits
        Zoo z(ConstructionData().setEnum<Zoo::Cat>(Zoo::Cat::TIGER).setParamter<Zoo::Name>("Hellabrunn"));
        ASSERT_TRUE(z.getName().value() == "Hellabrunn");
        ASSERT_TRUE(z.getCat() == Zoo::Cat::TIGER);
        ASSERT_TRUE(z.getMinVisitors() == 20);   // check default value
        ASSERT_TRUE(z.getMaxVisitors() == 8000); // check default value
    }
    {
        // min and max visitor numbers within the limits
        D_EXPECT_THROW(Zoo z(ConstructionData()
                                 .setEnum<Zoo::Cat>(Zoo::Cat::TIGER)
                                 .setParamter<Zoo::Name>("Hellabrunn")
                                 .setParamter<Zoo::Min_Visitors>(5)),
                       "is smaller than the allowed min value");
        D_EXPECT_THROW(Zoo z(ConstructionData()
                                 .setEnum<Zoo::Cat>(Zoo::Cat::TIGER)
                                 .setParamter<Zoo::Name>("Hellabrunn")
                                 .setParamter<Zoo::Min_Visitors>(50000)),
                       "is bigger than the allowed max value");
        D_EXPECT_THROW(Zoo z(ConstructionData()
                                 .setEnum<Zoo::Cat>(Zoo::Cat::TIGER)
                                 .setParamter<Zoo::Name>("Hellabrunn")
                                 .setParamter<Zoo::Max_Visitors>(50000)),
                       "is bigger than the allowed max value");
        D_EXPECT_THROW(Zoo z(ConstructionData()
                                 .setEnum<Zoo::Cat>(Zoo::Cat::TIGER)
                                 .setParamter<Zoo::Name>("Hellabrunn")
                                 .setParamter<Zoo::Max_Visitors>(0)),
                       "is smaller than the allowed min value");
    }
}

TEST_F(ConstructionValidatorTests, testSomethingForException)
{
    //W_EXPECT_THROW("do something exceptional", "something bad is going on");
}
