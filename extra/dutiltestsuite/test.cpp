#include "libdutil/%TestedClassName:l%.%CppHeaderSuffix%"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class %ClassName% : public TestBase
{};
}

TEST_F(%ClassName%, testSomethingForValueEquality)
{
    ASSERT_EQ("reference value", "actual value");
}

TEST_F(%ClassName%, testSomethingForException)
{
    //W_EXPECT_THROW("do something exceptional", "something bad is going on");
}
