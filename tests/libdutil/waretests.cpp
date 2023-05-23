#include "tests/testbase.h"
#include "libd/tests/testdummy.h"
#include "libd/libdutil/constructiondata.h"

using namespace DUTIL;

namespace {
class WareTests : public TestBase
{};
} // namespace

TEST_F(WareTests, getConcreteClassNameWorksAsExpected)
{
    LIBD::TESTS::TestDummy td(LIBD::TESTS::TestDummy::COLOR::GREEN);
    auto strName = "::TESTS::TestDummy";
    auto strShortName = "TestDummy";
    auto name = td.getConcreteClassName();
    auto shortName = td.getShortConcreteClassName();
    EXPECT_EQ(name, strName);
    EXPECT_EQ(shortName, strShortName);
}
