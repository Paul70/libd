#include "libd/libdutil/namedclass.h"
#include "tests/libtesting/testdummy.h"
#include "tests/testbase.h"

using namespace LIBD::TESTS;

namespace {
class NamedClassTests : public TestBase
{};
}  // namespace

TEST_F(NamedClassTests, namedClassMakroWorksAsExpected)
{
  auto longClassName = TestDummy::getClassName();
  auto shortClassName = TestDummy::getShortClassName();

  EXPECT_EQ(longClassName, "::LIBD::TESTS::TestDummy");
  EXPECT_EQ(shortClassName, "TestDummy");
}
