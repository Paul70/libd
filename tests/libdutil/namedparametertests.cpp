#include "tests/testbase.h"
#include "libd/libdutil/namedparameter.h"
#include "libd/libdutil/ware.h"

using namespace DUTIL;

namespace TEST_NamedParameter {
class TestClass
{
public:
    D_NAMED_BOOL(TestClass_Bool);
    D_NAMED_STRING(TestClass_String);
    D_NAMED_STRING(TestClass_String_With_Value);

    TestClass() :
        tcStrVal_(""),
        tcb_() // einen leeren ctor gibt es also
    {}

    TestClass(std::string s) :
        tcStrVal_(s)
    {}

    TestClass_String_With_Value tcStrVal_;
    TestClass_Bool tcb_;
};
} // namespace TEST_NamedParameter

namespace {
class NamedParameterTests : public TestBase
{};
} // namespace

TEST_F(NamedParameterTests, NamedBoolWorksAsExpected)
{
    std::string str = "TestClass_Bool";
    std::string name = TEST_NamedParameter::TestClass::TestClass_Bool::getParameterName();
    EXPECT_EQ(str, name);
}

TEST_F(NamedParameterTests, NamedStringWorksAsExpected)
{
    std::string str = "TestClass_String";
    std::string name = TEST_NamedParameter::TestClass::TestClass_String::getParameterName();
    EXPECT_EQ(str, name);

    TEST_NamedParameter::TestClass tc("Hello World!");
    std::string value = tc.tcStrVal_.value();
    EXPECT_EQ(value, "Hello World!");

    [[maybe_unused]] Ware w;
}
