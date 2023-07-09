#include "libdutil/namedparameter.h"
#include "libdutil/settingrule.h"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class SettingRuleTests : public TestBase
{};

D_NAMED_ENUM(Weekday, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY)
D_NAMED_STRING(StringWithName)
D_NAMED_REAL(RealWithName)
D_NAMED_LABEL(LabelWithName)
D_NAMED_BOOL(BoolWithName)

} // namespace

TEST_F(SettingRuleTests, testConstructDefaultSettingRuleWorksAsExpected)
{
    SettingRule sr;
    ASSERT_EQ(sr.usage, SettingRule::Usage::OPTIONAL);
    ASSERT_EQ(sr.description, "");
    ASSERT_EQ(sr.key, "");
    ASSERT_EQ(sr.defaultValue, Variant());
    ASSERT_EQ(sr.minimalValue, Variant());
    ASSERT_EQ(sr.maximalValue, Variant());
    ASSERT_EQ(sr.minimalStringLength, 0);
    ASSERT_EQ(sr.listOfPossibleValues, StringList());
    ASSERT_EQ(sr.type, Variant::Type::STRING);
}

TEST_F(SettingRuleTests, testCreateSettingRuleForNamedEnumWorksAsExpected)
{
    using SR = SettingRule;
    SR sr = SR::forNamedEnum<Weekday>(SR::Usage::MANDATORY_WITH_DEFAULT, "This is the description for named enum 'Weekday'");
    sr.defaultValue = Variant(Weekday::MONDAY);

    ASSERT_EQ(sr.usage, SettingRule::Usage::MANDATORY_WITH_DEFAULT);
    ASSERT_EQ(sr.description, "This is the description for named enum 'Weekday'");
    ASSERT_EQ(sr.key, Weekday::getEnumName());
    ASSERT_EQ(Weekday(sr.defaultValue), Weekday::MONDAY);
    ASSERT_EQ(sr.minimalValue, Variant());
    ASSERT_EQ(sr.maximalValue, Variant());
    ASSERT_EQ(sr.minimalStringLength, 0);
    ASSERT_EQ(sr.listOfPossibleValues, Weekday::getAllowedNames());
    ASSERT_EQ(sr.type, Variant::Type::STRING);
}

TEST_F(SettingRuleTests, testCreateSettingRuleForNamedParameterWorksAsExpected)
{
    using SR = SettingRule;
    // for named String
    {
        SR sr = SR::forNamedParameter<StringWithName>(SR::Usage::MANDATORY_WITH_DEFAULT,
                                                      "This is the description for named string parameter");
        sr.defaultValue = Variant("Hello World!");
        sr.minimalStringLength = 5;

        ASSERT_EQ(sr.usage, SettingRule::Usage::MANDATORY_WITH_DEFAULT);
        ASSERT_EQ(sr.description, "This is the description for named string parameter");
        ASSERT_EQ(sr.key, StringWithName::getParameterName());
        ASSERT_EQ(sr.defaultValue.toString(), "Hello World!");
        ASSERT_EQ(sr.minimalValue, Variant());
        ASSERT_EQ(sr.maximalValue, Variant());
        ASSERT_EQ(sr.minimalStringLength, 5);
        ASSERT_EQ(sr.listOfPossibleValues, StringList());
        ASSERT_EQ(sr.type, Variant::Type::STRING);
    }
    // for named real
    {
        SR sr = SR::forNamedParameter<RealWithName>(SR::Usage::MANDATORY_WITH_DEFAULT,
                                                    "This is the description for named real parameter");
        sr.defaultValue = Variant(7.77);
        sr.minimalStringLength = 5;

        ASSERT_EQ(sr.usage, SettingRule::Usage::MANDATORY_WITH_DEFAULT);
        ASSERT_EQ(sr.description, "This is the description for named real parameter");
        ASSERT_EQ(sr.key, RealWithName::getParameterName());
        ASSERT_EQ(sr.defaultValue.toReal(), 7.77);
        ASSERT_EQ(sr.minimalValue, Variant(std::numeric_limits<real_t>::lowest()));
        ASSERT_EQ(sr.maximalValue, Variant(std::numeric_limits<real_t>::max()));
        ASSERT_EQ(sr.minimalStringLength, 5);
        ASSERT_EQ(sr.listOfPossibleValues, StringList());
        ASSERT_EQ(sr.type, Variant::Type::DOUBLE);
    }
    // for named label
    {
        SR sr = SR::forNamedParameter<LabelWithName>(SR::Usage::MANDATORY_WITH_DEFAULT,
                                                     "This is the description for named label parameter");
        sr.defaultValue = Variant(700);
        sr.minimalStringLength = 5;
        sr.minimalValue = Variant(100);
        sr.maximalValue = Variant(1000);

        ASSERT_EQ(sr.usage, SettingRule::Usage::MANDATORY_WITH_DEFAULT);
        ASSERT_EQ(sr.description, "This is the description for named label parameter");
        ASSERT_EQ(sr.key, LabelWithName::getParameterName());
        ASSERT_EQ(sr.defaultValue.toLabel(), 700);
        ASSERT_EQ(sr.minimalValue.toLabel(), 100);
        ASSERT_EQ(sr.maximalValue.toLabel(), 1000);
        ASSERT_EQ(sr.minimalStringLength, 5);
        ASSERT_EQ(sr.listOfPossibleValues, StringList());
        ASSERT_EQ(sr.type, Variant::Type::LABEL);
    }
    // for named bool
    {
        SR sr = SR::forNamedParameter<BoolWithName>(SR::Usage::OPTIONAL, "This is the description for named bool parameter");
        sr.defaultValue = Variant(true);
        sr.minimalStringLength = 5;

        ASSERT_EQ(sr.usage, SettingRule::Usage::OPTIONAL);
        ASSERT_EQ(sr.description, "This is the description for named bool parameter");
        ASSERT_EQ(sr.key, BoolWithName::getParameterName());
        ASSERT_EQ(sr.defaultValue.toBool(), true);
        ASSERT_EQ(sr.minimalValue, Variant());
        ASSERT_EQ(sr.maximalValue, Variant());
        ASSERT_EQ(sr.minimalStringLength, 5);
        ASSERT_EQ(sr.listOfPossibleValues, StringList());
        ASSERT_EQ(sr.type, Variant::Type::BOOL);
    }
}
