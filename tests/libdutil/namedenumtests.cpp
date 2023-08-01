#include "libdutil/constructiondata.h"
#include "libdutil/namedenum.h"
#include "libdutil/variant.h"
#include "tests/testbase.h"
#include "tests/testdummy.h"

using namespace DUTIL;

namespace {
class NamedEnumTests : public TestBase
{};
} // namespace

TEST_F(NamedEnumTests, testConstructionWorksAsExpected)
{
    using namespace LIBD;
    using namespace TESTS;

    ConstructionData cd = ConstructionData().setEnum(TestDummy::COLOR::GREEN);
    TestDummy td(cd);
    auto nev = td.getNamedEnum();
    EXPECT_EQ(nev, TestDummy::COLOR::GREEN);

    // default construction, default value is first given enum value
    {
        TestDummy::WEEKDAY e_wd{};
        EXPECT_EQ(e_wd, TestDummy::WEEKDAY::FRIDAY);
        TestDummy::COLOR e_col{};
        EXPECT_EQ(e_col, TestDummy::COLOR::RED);
    }

    // construct form base type value which is int in that case
    {
        TestDummy::WEEKDAY e_wd{0};
        // conversion with implicit cast
        EXPECT_EQ((TestDummy::WEEKDAY) e_wd, TestDummy::WEEKDAY::FRIDAY);
        // conversion with static cast
        EXPECT_EQ(static_cast<TestDummy::WEEKDAY>(e_wd), TestDummy::WEEKDAY::FRIDAY);

        TestDummy::COLOR e_col{10}; // initialization of variable with COLOR::BLUE which corresponds to base value int, 10
        EXPECT_EQ(static_cast<TestDummy::COLOR>(e_col), TestDummy::COLOR::BLUE);
    }

    // construct from enum map string
    {
        TestDummy::WEEKDAY e_wd{"SUNDAY"};
        // conversion with static cast
        EXPECT_EQ(static_cast<TestDummy::WEEKDAY>(e_wd), TestDummy::WEEKDAY::SUNDAY);

        TestDummy::COLOR e_col{"GREEN"};
        EXPECT_EQ(static_cast<TestDummy::COLOR>(e_col), TestDummy::COLOR::GREEN);
    }

    // construct from DUTIL::Variant
    {
        DUTIL::Variant var("SATURDAY");
        TestDummy::WEEKDAY e_wd(var);
        // conversion with static cast
        EXPECT_EQ(static_cast<TestDummy::WEEKDAY>(e_wd), TestDummy::WEEKDAY::SATURDAY);

        DUTIL::Variant var2("GREEN");
        TestDummy::COLOR e_col{var2};
        EXPECT_EQ(static_cast<TestDummy::COLOR>(e_col), TestDummy::COLOR::GREEN);

        // construct a named enum form a variant containing a named enum
        DUTIL::Variant var3{e_col};
        TestDummy::COLOR e_col2{var3};
        EXPECT_EQ(static_cast<TestDummy::COLOR>(e_col2), TestDummy::COLOR::GREEN);
    }

    // expect throw due to invalid initialization
    {
        DUTIL::Variant var("HOLIDAY");
        D_EXPECT_THROW(TestDummy::WEEKDAY e_wd(var), "is not registered as an enum value");
        D_EXPECT_THROW(TestDummy::WEEKDAY e_wd(100), "Attempted initialization of named enum");
    }
}

TEST_F(NamedEnumTests, testGetValueListWorksAsExpected)
{
    using namespace LIBD::TESTS;
    ConstructionData cd = ConstructionData().setEnum(TestDummy::COLOR::GREEN);
    LIBD::TESTS::TestDummy td(cd);
    auto nev = td.getNamedEnum();
    auto colors = nev.getValueList();
    EXPECT_EQ(colors, "RED = 5, BLUE = 10, GREEN");
}

TEST_F(NamedEnumTests, testFromEnumValueWorksAsExpected)
{
    auto ne = fromEnumValue(LIBD::TESTS::TestDummy::WEEKDAY::FRIDAY);
    auto name = ne.getEnumName();
    EXPECT_EQ(name, "WEEKDAY");
}

TEST_F(NamedEnumTests, testToStringWorksAsExpected)
{
    auto enumString = toString(LIBD::TESTS::TestDummy::WEEKDAY::FRIDAY);
    EXPECT_EQ(enumString, "FRIDAY");
    auto ne = fromEnumValue(LIBD::TESTS::TestDummy::WEEKDAY::SUNDAY);
    enumString = toString(ne.value());
    EXPECT_EQ(enumString, "SUNDAY");
}

TEST_F(NamedEnumTests, testCheckTypeValuePair)
{
    using namespace LIBD;
    using namespace TESTS;

    TestDummy::WEEKDAY e_wd{};
    EXPECT_EQ(static_cast<int>(e_wd), 0);
    TestDummy::COLOR e_col{};
    EXPECT_EQ(static_cast<int>(e_col), 5);

    // to be able to code the following lines, it is necessary to have in implicit conversion operator.
    EXPECT_EQ(static_cast<int>(TestDummy::COLOR::BLUE), 10);
    EXPECT_EQ(static_cast<int>(TestDummy::WEEKDAY::SUNDAY), 2);
    EXPECT_EQ(static_cast<int>(TestDummy::WEEKDAY::SATURDAY), 1);
    EXPECT_EQ(static_cast<int>(TestDummy::WEEKDAY::FRIDAY), 0);
}

TEST_F(NamedEnumTests, testGetAllowedNamesWorksAsExpected)
{
    using namespace LIBD;
    using namespace TESTS;

    auto names = TestDummy::COLOR::getAllowedNames();
    auto number = TestDummy::COLOR::size();
    EXPECT_EQ(names[0], "RED");
    EXPECT_EQ(names[1], "BLUE");
    EXPECT_EQ(names[2], "GREEN");
    EXPECT_EQ(number, 3);
}

TEST_F(NamedEnumTests, testPublicToStringWorksAsExpected)
{
    using namespace LIBD;
    using namespace TESTS;

    ConstructionData cd = ConstructionData().setEnum(TestDummy::COLOR::GREEN);
    TestDummy td(cd);
    auto nev = td.getNamedEnum();
    EXPECT_EQ(nev.toString(), "GREEN");
}
