#include "libd/libdutil/constructiondata.h"
#include "libd/tests/testdummy.h"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class WareTests : public TestBase
{};
} // namespace

TEST_F(WareTests, getConcreteClassNameWorksAsExpected)
{
    using namespace LIBD::TESTS;
    ConstructionData cd = ConstructionData().setEnum(TestDummy::COLOR::GREEN);
    TestDummy td(cd);
    auto strName = "::TESTS::TestDummy";
    auto strShortName = "TestDummy";
    auto name = td.getConcreteClassName();
    auto shortName = td.getShortConcreteClassName();
    EXPECT_EQ(name, strName);
    EXPECT_EQ(shortName, strShortName);
}
