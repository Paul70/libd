#include "libd/libdutil/namedparameter.h"
#include "libd/libdutil/ware.h"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class NamedParameterTests : public TestBase
{};

D_NAMED_BOOL(BoolWithName)
D_NAMED_STRING(StringWithName)
D_NAMED_LABEL(LabelWithName)
D_NAMED_REAL(RealWithName)

class TestClass
{
public:
    D_NAMED_BOOL(TestClass_Bool)
    D_NAMED_LABEL(TestClass_Label)
    D_NAMED_REAL(TestClass_Real)
    D_NAMED_STRING(TestClass_String)
    D_NAMED_STRING(TestClass_String_With_Value)

    //! default constructor
    TestClass() :
        tcStrVal_(),
        tcb_(),
        tl_(),
        tr_()
    {}

    TestClass(std::string s, bool b, label_t l, real_t r) :
        tcStrVal_(s),
        tcb_(b),
        tl_(l),
        tr_(r)
    {}

    TestClass_String_With_Value tcStrVal_;
    TestClass_Bool tcb_;
    TestClass_Label tl_;
    TestClass_Real tr_;
};

} // namespace

TEST_F(NamedParameterTests, NamedBoolWorksAsExpected)
{
    std::string str = "TestClass_Bool";
    std::string name = TestClass::TestClass_Bool::getParameterName();
    EXPECT_EQ(str, name);

    {
        TestClass tc("Hello World!", true, 7, 7.77);
        EXPECT_EQ(true, tc.tcb_);
    }

    {
        Variant varBool(true);
        BoolWithName bwn(varBool);
        ASSERT_EQ(varBool.toBool(), bwn.value());
    }
}

TEST_F(NamedParameterTests, NamedStringWorksAsExpected)
{
    std::string str = "TestClass_String";
    std::string name = TestClass::TestClass_String::getParameterName();
    EXPECT_EQ(str, name);

    {
        TestClass tc("Hello World!", true, 7, 7.77);
        std::string value = tc.tcStrVal_.value();
        EXPECT_EQ(value, "Hello World!");
    }
    {
        Variant var("Hello World!");
        StringWithName swn(var);
        ASSERT_EQ(var.toString(), swn.value());
    }
}

TEST_F(NamedParameterTests, testNamedLabelWorksAsExpected)
{
    std::string str = "TestClass_Label";
    std::string name = TestClass::TestClass_Label::getParameterName();
    EXPECT_EQ(str, name);

    {
        TestClass tc("Hello World!", true, 7, 7.77);
        label_t value = tc.tl_.value();
        EXPECT_EQ(value, 7);
    }
    {
        Variant var(7);
        LabelWithName lwn(var);
        ASSERT_EQ(var.toLabel(), lwn.value());
    }
}

TEST_F(NamedParameterTests, testNamedRealWorksAsExpected)
{
    std::string str = "TestClass_Real";
    std::string name = TestClass::TestClass_Real::getParameterName();
    EXPECT_EQ(str, name);

    {
        TestClass tc("Hello World!", true, 7, 7.77);
        real_t value = tc.tr_.value();
        EXPECT_EQ(value, 7.77);
    }
    {
        Variant var(7.77);
        RealWithName rwn(var);
        ASSERT_EQ(var.toReal(), rwn.value());
    }
}
