#include "libdutil/constructiondata.h"
#include "libdutil/settings.h"
#include "tests/testbase.h"
#include "tests/testdummy.h"

using namespace DUTIL;

namespace {
class ConstructionDataTests : public TestBase
{};

D_NAMED_ENUM(WEEKDAY, FRIDAY, SATURDAY, SUNDAY);
} // namespace

TEST_F(ConstructionDataTests, testSetWorksAsExpected)
{
    Settings s = Settings().set("String", "Hello World");
    s.set("DoubelA", 123.456);
    s.set("Label_t", 7);

    ConstructionData cd;
    cd.set(s);
    ASSERT_EQ(cd.s, s);                        // lvalue call
    ASSERT_EQ(ConstructionData().set(s).s, s); // rvalue call

    cd.setEnum(WEEKDAY::SUNDAY);
    ASSERT_TRUE(cd.s.hasKey("WEEKDAY"));

    ConstructionData cd2;
    ASSERT_NE(cd2.s, cd.s); // test for unequality
}

TEST_F(ConstructionDataTests, testConstructWithConstructionDataWorksAsExpected)
{
    using namespace LIBD::TESTS;
    ConstructionData cd = ConstructionData().setEnum(TestDummy::COLOR::GREEN);
    TestDummy td(cd);
    ASSERT_EQ(td.getNamedEnum(), TestDummy::COLOR::GREEN);
}

TEST_F(ConstructionDataTests, testSomethingForException)
{
    //W_EXPECT_THROW("do something exceptional", "something bad is going on");
}
