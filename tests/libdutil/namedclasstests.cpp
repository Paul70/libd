#include "tests/testbase.h"
#include "tests/testdummy.h"
#include "libd/libdutil/namedclass.h"

using namespace LIBD::TESTS;

namespace {
class NamedClassTests : public TestBase
{};
} // namespace

TEST_F(NamedClassTests, namedClassMakroWorksAsExpected)
{
    auto longClassName = TestDummy::getClassName();
    auto shortClassName = TestDummy::getShortClassName();

    EXPECT_EQ(longClassName, "::TESTS::TestDummy");
    EXPECT_EQ(shortClassName, "TestDummy");
}
