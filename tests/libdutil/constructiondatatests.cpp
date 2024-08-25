#include "libd/libdutil/constructiondata.h"
#include "libd/libdutil/constructionvalidator.h"
#include "libd/libdutil/namedreference.h"
#include "libd/libdutil/settings.h"
#include "tests/libtesting/compoundware.h"
#include "tests/libtesting/trivialware.h"
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

  static ConstructionValidator const& getConstructionValidator()
  {
    static ConstructionValidator cv;
    return cv;
  }

  explicit SubSubobject(ConstructionData const& cd) :
      color_(cd.s.getEnum<COLOR>()),
      descr_(cd.s.getParameter<Description>())
  {}

  COLOR getColor() const { return color_; }

  Description getDescription() const { return descr_; }

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

  static ConstructionValidator const& getConstructionValidator()
  {
    static ConstructionValidator cv;
    return cv;
  }

  explicit Subobject(ConstructionData const& cd) :
      subsubList_(),
      descr_(cd.s.getParameter<Description>())
  {}

  Description getDescription() const { return descr_; }

  private:
  SubSubObjectList subsubList_;
  Description descr_;
};

class Object
{
  public:
  private:
};

}  // namespace

TEST_F(ConstructionDataTests, testSetWorksAsExpected)
{
  Settings s = Settings().set("String", "Hello World");
  s.set("DoubelA", 123.456);
  s.set("Label_t", 7);

  ConstructionData cd;
  cd.set(s);
  ASSERT_EQ(cd.s, s);                         // lvalue call
  ASSERT_EQ(ConstructionData().set(s).s, s);  // rvalue call

  cd.setEnum(WEEKDAY::SUNDAY);
  ASSERT_TRUE(cd.s.hasKey("WEEKDAY"));

  ConstructionData cd2;
  ASSERT_NE(cd2.s, cd.s);  // test for unequality
}

TEST_F(ConstructionDataTests, testConstructWithConstructionDataWorksAsExpected)
{
  // ConstructionData for SubSubObject
  // clang-format off
    ConstructionData cd = ConstructionData()
                            .setEnum<SubSubobject::COLOR>(SubSubobject::COLOR::GREEN)
                            .setParameter<SubSubobject::Description>("This is a Subsubobject");

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
                            .setParameter<Subobject::Description>("This is a SubObject")
                            .addSubobject<Subobject::SubSubObjectList>(ConstructionData()
                                                                           .setEnum<SubSubobject::COLOR>(SubSubobject::COLOR::GREEN)
                                                                           .setParameter<SubSubobject::Description>("This is a SubSubObject"));
  // clang-format on
  ConstructionData cdForSubSubObject = cd.getSubObjectCd<Subobject::SubSubObjectList>();
  SubSubobject sso = SubSubobject(cdForSubSubObject);
  ASSERT_EQ(sso.getColor().value(), SubSubobject::COLOR::GREEN);
  ASSERT_EQ(sso.getDescription().value(), "This is a SubSubObject");
}

TEST_F(ConstructionDataTests, setSettingsUsingLvalueReference)
{
  ConstructionData cd;
  cd.set(Settings().set("hello", "world"));
  ASSERT_EQ("world", cd.s.value("hello").toString());
}

TEST_F(ConstructionDataTests, setSettingsUsingRvalueReference)
{
  ConstructionData cd = ConstructionData().set(Settings().set("hello", "world"));
  ASSERT_EQ("world", cd.s.value("hello").toString());
}

TEST_F(ConstructionDataTests, setDatasetUsingLvalueReference)
{
  ConstructionData cd;
  cd.set(Dataset(std::vector<label_t>(5, 10)));
  ASSERT_EQ(std::vector<label_t>(5, 10), cd.ds.getValues<label_t>());
}

TEST_F(ConstructionDataTests, setDatasetUsingRvalueReference)
{
  ConstructionData cd = ConstructionData().set(Dataset(std::vector<label_t>(5, 10)));
  ASSERT_EQ(std::vector<label_t>(5, 10), cd.ds.getValues<label_t>());
}

TEST_F(ConstructionDataTests, setEnum)
{
  ConstructionData cd = ConstructionData().setEnum(TESTS::CompoundWareC::ChoiceC::THE_BAD);
  ASSERT_EQ(fromEnumValue(TESTS::CompoundWareC::ChoiceC::THE_BAD).toString(),
            cd.s.value(TESTS::CompoundWareC::ChoiceC::getEnumName()).toString());

  cd.setEnum(TESTS::CompoundWareC::ChoiceC::THE_UGLY);
  ASSERT_EQ(fromEnumValue(TESTS::CompoundWareC::ChoiceC::THE_UGLY).toString(),
            cd.s.value(TESTS::CompoundWareC::ChoiceC::getEnumName()).toString());
}

TEST_F(ConstructionDataTests, setParameter_worksAsExpected)
{
  ConstructionData cd = ConstructionData().setParameter<TESTS::CompoundWareA::LabelA>(-55);
  ASSERT_EQ(-55, cd.s.value(TESTS::CompoundWareA::LabelA::getParameterName()).toLabel());
}

TEST_F(ConstructionDataTests, setConcreteClassParameter_worksAsExpected)
{
  ConstructionData cd = ConstructionData().setConcreteClassParameter<TESTS::CompoundWareA>();
  ASSERT_EQ(TESTS::CompoundWareA::getClassName(),
            cd.s.value(Ware::DUTIL_Ware_Type::getParameterName()).toString());
}

TEST_F(ConstructionDataTests, addSharedWare_worksAsExpected)
{
  D_NAMED_REFERENCE(TWRef, TESTS::TrivialWare);
  D_NAMED_REFERENCE(Blub, TESTS::TrivialWare);
  ConstructionData cd;
  ASSERT_TRUE(cd.sharedWares.empty());

  cd.addSharedWare(TWRef("twref0"));
  ASSERT_EQ(1u, cd.sharedWares.size());
  ASSERT_NE(cd.getSharedWareWithCounter("TWRef", 0), cd.sharedWares.cend());
  ASSERT_FALSE(bool(cd.sharedWares.at("TWRef;0")));

  auto twref1 = std::make_shared<TESTS::TrivialWare>();
  cd.addSharedWare(TWRef("twref1", twref1));
  cd.addSharedWare(Blub("blub0", std::make_shared<TESTS::TrivialWare>()));
  ASSERT_EQ(3u, cd.sharedWares.size());

  ASSERT_NE(cd.getSharedWareWithCounter("TWRef", 0), cd.sharedWares.cend());
  ASSERT_NE(cd.getSharedWareWithCounter("TWRef", 1), cd.sharedWares.cend());
  ASSERT_NE(cd.getSharedWareWithCounter("Blub", 0), cd.sharedWares.cend());
  ASSERT_FALSE(bool(cd.sharedWares.at("TWRef;0")));
  ASSERT_EQ(twref1, cd.sharedWares.at("TWRef;1"));
  ASSERT_TRUE(bool(cd.sharedWares.at("Blub;0")));

  cd.addSharedWare(TWRef("twref1_new", twref1));
  ASSERT_EQ(3u, cd.sharedWares.size());
  ASSERT_EQ(twref1, cd.sharedWares.at("TWRef;1"));
}

TEST_F(ConstructionDataTests, getSubobject_noDataPresent_returnsEmptyConstructionData)
{
  D_NAMED_REFERENCE(TWRef, TESTS::TrivialWare);
  ConstructionData cd;
  ASSERT_TRUE(cd.subObjectData.empty());
  ConstructionData childCD = cd.getSubObjectCd<TWRef>();
  ASSERT_EQ(childCD.ds, Dataset());
  ASSERT_EQ(childCD.s, Settings());
  ASSERT_TRUE(childCD.subObjectData.empty());
}

TEST_F(ConstructionDataTests, getSubobject_dataPresent_works)
{
  // geht noch nicht, da muss ich das deconstruct implementieren
  //  D_NAMED_REFERENCE(WareARef, TESTS::CompoundWareA);
  //  TESTS::CompoundWareA wareA(ConstructionData().setParameter<TESTS::CompoundWareA::LabelA>(-44));
  //  ConstructionData cd;
  //  cd.addSubobject<WareARef>(std::move(wareA));

  //  ConstructionData wareACD = cd.getSubobject<WareARef>();
  //  ASSERT_EQ(wareACD.s.getParameter<TESTS::CompoundWareA::LabelA>(), -44);
}

TEST_F(ConstructionDataTests, addSubobject_using_deconstructable_object_worksAsExpected)
{
  //  W_NAMED_REFERENCE(WareARef, CompoundWareA);
  //  CompoundWareA wareA(ConstructionData().setParameter<CompoundWareA::LabelA>(-44));
  //  ConstructionData cd;
  //  ASSERT_TRUE(cd.subobjectData.empty());

  //  cd.addSubobject<WareARef>(std::move(wareA));
  //  ASSERT_EQ(1u, cd.subobjectData.size());
  //  ASSERT_NE(cd.getSubobjectWithCounter("WareARef", 0), cd.subobjectData.cend());

  //  ASSERT_EQ(cd.getSubobjectWithCounter("WareARef", 0)->second.s,
  //            Settings().setParameter<CompoundWareA::LabelA>(-44));
}

TEST_F(ConstructionDataTests, createSharedWareKeyWithCounter)
{
  ConstructionData cd;
  auto tw0 = std::make_shared<TESTS::TrivialWare>();
  ASSERT_EQ(cd.createSharedWareKeyWithCounter("bla", tw0), "bla;0");
}

TEST_F(ConstructionDataTests, createSubobjectKeyWithCounter)
{
  ConstructionData cd;
  ASSERT_EQ(cd.createSubObjectKeyWithCounter("bla"), "bla;0");
}
