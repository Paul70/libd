#include "libdutil/constructiondata.h"
#include "libdutil/namedenum.h"
#include "tests/testbase.h"
#include "tests/testdummy.h"

using namespace DUTIL;

namespace {
class NamedEnumTests : public TestBase
{};
} // namespace

TEST_F(NamedEnumTests, testNamedEnumMacroWorksAsExpected)
{
    LIBD::TESTS::TestDummy td(LIBD::TESTS::TestDummy::COLOR::GREEN);
    auto ne = td.getNamedEnum();
    auto colors = ne.getValueList();
    EXPECT_TRUE(1);
}

//TEST_F(NamedEnumTests, testSomethingForValueEquality)
//{
//    ASSERT_EQ("reference value", "actual value");
//}

//TEST_F(NamedEnumTests, testSomethingForException)
//{
//    W_EXPECT_THROW("do something exceptional", "something bad is going on");
//}
