#include "libdutil/constructionvalidator.h"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class ConstructionValidatorTests : public TestBase
{};
} // namespace

TEST_F(ConstructionValidatorTests, testSomethingForValueEquality)
{
    //ASSERT_EQ("reference value", "actual value");
}

TEST_F(ConstructionValidatorTests, testSomethingForException)
{
    //W_EXPECT_THROW("do something exceptional", "something bad is going on");
}
