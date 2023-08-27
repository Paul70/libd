#include "libdutil/constructiondata.h"
#include "libdutil/namedreference.h"
#include "libdutil/settings.h"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class ConstructionDataTests : public TestBase
{};

//! Definition of named enum type for test purposes.
D_NAMED_ENUM(WEEKDAY, FRIDAY, SATURDAY, SUNDAY)

//! Definition of classes for test purposes. Class SubSubObject only needs construction data containing settings.
class SubSubobject
{
public:
    D_NAMED_ENUM(COLOR, RED = 5, BLUE = 10, GREEN)
    D_NAMED_STRING(Description)

    explicit SubSubobject(ConstructionData const &cd) :
        color_(cd.s.getEnum<COLOR>()),
        descr_(cd.s.getParameter<Description>())
    {}

    COLOR getColor() const
    {
        return color_;
    }
    Description getDescription() const
    {
        return descr_;
    }

private:
    COLOR color_;
    Description descr_;
};

//! Class Subobject needs conctruction data containing a nested ConstructionData object for
//! referenced member from type SubSubObject and containing settings.
class Subobject
{
public:
    //! Define the list of sub-subobjects needed for construction
    D_NAMED_REFERENCE(SubSubObjectList, SubSubobject)
    D_NAMED_STRING(Description)

    explicit Subobject(ConstructionData const &cd) :
        subsubList_(),
        descr_(cd.s.getParameter<Description>())
    {}

    Description getDescription() const
    {
        return descr_;
    }

private:
    SubSubObjectList subsubList_;
    Description descr_;
};

class Object
{
public:
private:
};

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
    // ConstructionData for SubSubObject
    // clang-format off
    ConstructionData cd = ConstructionData()
                            .setEnum<SubSubobject::COLOR>(SubSubobject::COLOR::GREEN)
                            .setParamter<SubSubobject::Description>("This is a Subsubobject");

    // clang-format on
    SubSubobject sso(cd);
    ASSERT_EQ(sso.getColor().value(), SubSubobject::COLOR::GREEN);
    ASSERT_EQ(sso.getDescription().value(), "This is a Subsubobject");
}

TEST_F(ConstructionDataTests, testAddSubObject)
{
    // ConstructionData for SubSubObject
    // clang-format off
    ConstructionData cd = ConstructionData()
                            .setParamter<Subobject::Description>("This is a SubObject")
                            .addSubObject<Subobject::SubSubObjectList>(ConstructionData()
                                                                           .setEnum<SubSubobject::COLOR>(SubSubobject::COLOR::GREEN)
                                                                           .setParamter<SubSubobject::Description>("This is a SubSubObject"));
    // clang-format on
    ConstructionData cdForSubSubObject = cd.getSubObjectCd<Subobject::SubSubObjectList>();
    SubSubobject sso = SubSubobject(cdForSubSubObject);
    ASSERT_EQ(sso.getColor().value(), SubSubobject::COLOR::GREEN);
    ASSERT_EQ(sso.getDescription().value(), "This is a SubSubObject");
}

TEST_F(ConstructionDataTests, testSomethingForException)
{
    //W_EXPECT_THROW("do something exceptional", "something bad is going on");
}
