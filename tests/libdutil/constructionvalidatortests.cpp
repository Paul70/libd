#include "libdutil/constructiondata.h"
#include "tests/compoundware.h"
#include "tests/factorytestsexampleclass.h"
#include "tests/testbase.h"
#include "tests/trivialware.h"
#include "tests/zoo.h"

using namespace TESTS;
using namespace DUTIL;

namespace {
class ConstructionValidatorTests : public TestBase
{};

D_NAMED_LABEL(LabelParam);
D_NAMED_REAL(RealParam);
D_NAMED_STRING(StringParam);
D_NAMED_REFERENCE(BaseReference, Base);
D_NAMED_REFERENCE(TrivialReference, TrivialWare);
D_NAMED_ENUM(States, NEWYORK, ALABAMA, CALIFORNIA, NEVADA, WOSHINGTON, FLORIDA, TEXAS)
D_NAMED_ENUM(Fruits, APPLE, ORANGE, BANANA);

}  // namespace

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Unit test using class Zoo (zoo.h) and Cat (cat.h) and building a more complex object compoition
//! with a real world reference

TEST_F(ConstructionValidatorTests, testB)
{
  ConstructionData cd;
  KK k;
  auto cv = KK::getConstructionValidator();
}

TEST_F(ConstructionValidatorTests, testDefaultConstruction)
{
  ConstructionData cd;
  ConstructionValidator cv = ConstructionValidator();
  ASSERT_TRUE(cv.getListOfSettingRuleKeys().empty());
  ASSERT_TRUE(cv.check(cd).empty());
}

TEST_F(ConstructionValidatorTests, testConstructionWithSettingRulesAndValidation)
{
  {
    ConstructionData cd = ConstructionData().setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::TUESDAY);
    cd.s.setParameter<Zoo::Name>("Hellabrunn");
    auto cv = Zoo::getConstructionValidator();
    auto validationError = cv.check(cd);
    ASSERT_TRUE(validationError.empty());
    Zoo z = Zoo(cd);
    ASSERT_TRUE(z.getOffDay() == Zoo::ClosingDay::TUESDAY);
  }
  {
    // zoo name not in list of possible values
    ConstructionData cd = ConstructionData().setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::TUESDAY);
    cd.s.setParameter<Zoo::Name>("Paul's Zoo");
    auto cv = Zoo::getConstructionValidator();
    auto validationError = cv.check(cd);
    ASSERT_FALSE(validationError.empty());
  }

  // test construction using the default value for Cat memeber
  {
    ConstructionData cd = ConstructionData().setParameter<Zoo::Name>("Wilhelma");
    auto cv = Zoo::getConstructionValidator();
    auto validationError = cv.check(cd);
    ASSERT_TRUE(validationError.empty());
    Zoo z = Zoo(cd);
    ASSERT_TRUE(z.getOffDay() == Zoo::ClosingDay::MONDAY);
    ASSERT_TRUE(z.getName().value() == "Wilhelma");
  }
  {
    // min and max visitor numbers within the limits
    Zoo z(ConstructionData()
              .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::FRIDAY)
              .setParameter<Zoo::Name>("Hellabrunn")
              .setParameter<Zoo::Min_Visitors>(50)
              .setParameter<Zoo::Max_Visitors>(1000));
    ASSERT_TRUE(z.getName().value() == "Hellabrunn");
    ASSERT_TRUE(z.getOffDay() == Zoo::ClosingDay::FRIDAY);
    ASSERT_TRUE(z.getMinVisitors() == 50);
    ASSERT_TRUE(z.getMaxVisitors() == 1000);
  }
  {
    // min and max visitor numbers within the limits
    Zoo z(ConstructionData()
              .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::SUNDAY)
              .setParameter<Zoo::Name>("Hellabrunn"));
    ASSERT_TRUE(z.getName().value() == "Hellabrunn");
    ASSERT_TRUE(z.getOffDay() == Zoo::ClosingDay::SUNDAY);
    ASSERT_TRUE(z.getMinVisitors() == 20);    // check default value
    ASSERT_TRUE(z.getMaxVisitors() == 8000);  // check default value
  }
  {
    // min and max visitor numbers within the limits
    auto result = D_EXPECT_THROW(Zoo z(ConstructionData()
                                           .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::TUESDAY)
                                           .setParameter<Zoo::Name>("Hellabrunn")
                                           .setParameter<Zoo::Min_Visitors>(5)),
                                 "is smaller than the allowed min value");
    EXPECT_THAT(result, testing::HasSubstr("is smaller than the allowed min value"));

    result = D_EXPECT_THROW(Zoo z(ConstructionData()
                                      .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::TUESDAY)
                                      .setParameter<Zoo::Name>("Hellabrunn")
                                      .setParameter<Zoo::Min_Visitors>(50000)),
                            "is bigger than the allowed max value");
    EXPECT_THAT(result, testing::HasSubstr("is bigger than the allowed max value"));

    result = D_EXPECT_THROW(Zoo z(ConstructionData()
                                      .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::TUESDAY)
                                      .setParameter<Zoo::Name>("Hellabrunn")
                                      .setParameter<Zoo::Max_Visitors>(50000)),
                            "is bigger than the allowed max value");
    EXPECT_THAT(result, testing::HasSubstr("is bigger than the allowed max value"));

    result = D_EXPECT_THROW(Zoo z(ConstructionData()
                                      .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::TUESDAY)
                                      .setParameter<Zoo::Name>("Hellabrunn")
                                      .setParameter<Zoo::Max_Visitors>(0)),
                            "is smaller than the allowed min value");
    EXPECT_THAT(result, testing::HasSubstr("is smaller than the allowed min value"));
  }
}

TEST_F(ConstructionValidatorTests, testBuildSubobject)
{
  // clang-format off
    Zoo z(ConstructionData()
              .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::TUESDAY)
              .setParameter<Zoo::Name>("Hellabrunn")
              .setParameter<Zoo::Min_Visitors>(50)
              .setParameter<Zoo::Max_Visitors>(1000)
              .addSubobject<Zoo::SingleTiger>(
                    ConstructionData()
                        .setConcreteClassParameter<Tiger>()
                        .setParameter<Cat::Name>("SingleTiger")
                        .setParameter<Cat::Age>(7))
          );
  // clang-format on
  ASSERT_TRUE(z.getOffDay() == Zoo::ClosingDay::TUESDAY);
  auto result = z.findCat("SingleTiger");
  ASSERT_TRUE(result);
}

TEST_F(ConstructionValidatorTests, testBuildSubobjectList)
{
  // clang-format off
    Zoo z(ConstructionData()
              .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::SATURDAY)
              .setParameter<Zoo::Name>("Hellabrunn")
              .setParameter<Zoo::Min_Visitors>(50)
              .setParameter<Zoo::Max_Visitors>(1000)
              .addSubobject<Zoo::CatList>(
                    ConstructionData()
                        .setConcreteClassParameter<Tiger>()
                        .setParameter<Tiger::Name>("TigerA")
                        .setParameter<Tiger::Age>(7))
              .addSubobject<Zoo::CatList>(
                    ConstructionData()
                        .setConcreteClassParameter<Tiger>()
                        .setParameter<Tiger::Name>("TigerB")
                        .setParameter<Tiger::Age>(8))
              .addSubobject<Zoo::CatList>(
                    ConstructionData()
                        .setConcreteClassParameter<Jaguar>()
                        .setParameter<Tiger::Name>("JaguarA")
                        .setParameter<Tiger::Age>(4))
              .addSubobject<Zoo::CatList>(
                    ConstructionData()
                        .setConcreteClassParameter<Jaguar>()
                        .setParameter<Tiger::Name>("JaguarB")
                        .setParameter<Tiger::Age>(4))
          );
  // clang-format on
  ASSERT_TRUE(z.getOffDay() == Zoo::ClosingDay::SATURDAY);
  ASSERT_TRUE((z.findCat("TigerA")));
  ASSERT_TRUE((z.findCat("TigerB")));
  ASSERT_TRUE((z.findCat("JaguarA")));
  ASSERT_TRUE((z.findCat("JaguarB")));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Unit test for testing subobjects and subobject lists

TEST_F(ConstructionValidatorTests, buildSubobjectWithOptionalUsage)
{
  D_NAMED_REFERENCE(RefToTW, TrivialWare);
  auto cv = ConstructionValidator({}, {[]() {
                                    auto wlr = WarelistRule::forSubobject<RefToTW>("unneeded ware");
                                    wlr.usage = WarelistRule::Usage::OPTIONAL;
                                    return wlr;
                                  }()});

  auto tw = cv.buildSubobject<RefToTW>(ConstructionData{});
  // We want a nullptr back since an empty construction data object cannot build an optional subobject
  ASSERT_FALSE(bool(tw));
}

TEST_F(ConstructionValidatorTests, buildSubobjectWithValidSubobjectData)
{
  ConstructionData cdA = ConstructionData().setParameter<CompoundWareA::LabelA>(-42);
  ConstructionData cdB = ConstructionData().addSharedWare(
      CompoundWareB::WareAPtrRef("wareAPtrRef", std::make_shared<CompoundWareA>(cdA)));
  ConstructionData cdC = ConstructionData().addSubobject<CompoundWareC::WareBInstance>(cdB);

  auto wareB
      = CompoundWareC::getConstructionValidator().buildSubobject<CompoundWareC::WareBInstance>(cdC);
  ASSERT_TRUE(wareB);
  ASSERT_EQ(-42, wareB->wareARef->lA);
}

TEST_F(ConstructionValidatorTests, buildSubobjectForInterfaceClass)
{
  D_NAMED_REFERENCE(InterfaceObject, Base);
  ConstructionValidator cv({}, {WarelistRule::forSubobject<InterfaceObject>("interface object")});

  auto cd = ConstructionData().addSubobject<InterfaceObject>(
      ConstructionData()
          .setConcreteClassParameter<DerivedA>()
          .setParameter<DerivedA::DerivedAInt>(42)
          .addSharedWare(Base::BaseWareAReference(
              "baseWareA", std::make_shared<CompoundWareA>(
                               ConstructionData().setParameter<CompoundWareA::LabelA>(13))))
          .addSubobject<Base::BaseWareAInstance>(
              ConstructionData().setParameter<CompoundWareA::LabelA>(17)));

  auto base = cv.buildSubobject<InterfaceObject>(cd);
  ASSERT_TRUE(bool(base));
  auto wareA = static_unique_ptr_cast<DerivedA>(std::move(base));
  ASSERT_TRUE(bool(wareA));
  ASSERT_EQ(42, wareA->id);
}

TEST_F(ConstructionValidatorTests, buildSubobjectListWithValidSubobjectData)
{
  // clang-format off
    ConstructionData cdA1 = ConstructionData().setParameter<CompoundWareA::LabelA>(1);
    ConstructionData cdA2 = ConstructionData().setParameter<CompoundWareA::LabelA>(2);
    ConstructionData cdC = ConstructionData()
                               .addSubobject<CompoundWareC::WareAVariableListInstance>(cdA1)
                               .addSubobject<CompoundWareC::WareAVariableListInstance>(cdA2);

    auto wareAList = CompoundWareC::getConstructionValidator().buildSubobjectList<CompoundWareC::WareAVariableListInstance>(cdC);
  // clang-format on
  ASSERT_EQ(2u, wareAList.size());
  ASSERT_EQ(1, wareAList[0]->lA);
  ASSERT_EQ(2, wareAList[1]->lA);
}

TEST_F(ConstructionValidatorTests, validateHeterogenousSubObjectsAndUndefinedListLength)
{
  {
    // build a CompundWareE object with one CompoundWareD in the list of subobjects.
    auto wareE = CompoundWareE(
        ConstructionData()  // CD ware E
            .setParameter<CompoundWareE::Description>(
                "CompundWareE: testing heterogenous subobjects "
                "and subobject list with undefined lenght.")
            .setParameter<CompoundWareBIntermediate::RealBIntermediate>(44.44)
            .addSubobject<CompoundWareE::WareDInstanceList>(
                ConstructionData()  // CD for ware D (only one in the list)
                    .setConcreteClassParameter<CompoundWareD>()
                    .setParameter<CompoundWareD::NameD>("D"))
            .addSubobject<CompoundWareBIntermediate::WareBInstance>(
                ConstructionData()  // CD B in Bintermediate
                    .setConcreteClassParameter<CompoundWareB>()
                    .setParameter<CompoundWareB::RealB>(7.77)
                    .addSharedWare(CompoundWareB::WareAPtrRef(
                        "wareAPtrRef",
                        std::make_shared<CompoundWareA>(
                            ConstructionData().setParameter<CompoundWareA::LabelA>(4))))));

    // CompoundWareE consists of:
    // - derived from CompoundWareBIntermediate and CompoundWareBIntermediate
    //   has a memeber variable CompundWareB
    // - CompoundWareB has a shared reference to a CompundWareA object.
    // - un undefined number of CompundWareD objects stored in vector
    ASSERT_EQ(wareE.dList.size(), 1);
    ASSERT_EQ(
        wareE.descr.value(),
        "CompundWareE: testing heterogenous subobjects and subobject list with undefined lenght.");
    ASSERT_EQ(wareE.dList.front()->name.value(), "D");
    ASSERT_EQ(wareE.wareB.rB, 7.77);
    ASSERT_EQ(wareE.wareB.wareARef->lA.value(), 4);
  }

  // build a CompundWareE object with 3 CompoundWareD in the list of subobjects.
  auto wareE = CompoundWareE(
      ConstructionData()  // CD ware E
          .setParameter<CompoundWareE::Description>("CompundWareE: testing heterogenous subobjects "
                                                    "and subobject list with undefined lenght.")
          .setParameter<CompoundWareBIntermediate::RealBIntermediate>(44.44)
          .addSubobject<CompoundWareE::WareDInstanceList>(
              ConstructionData()  // CD for first ware D
                  .setConcreteClassParameter<CompoundWareD>()
                  .setParameter<CompoundWareD::NameD>("D1"))
          .addSubobject<CompoundWareE::WareDInstanceList>(
              ConstructionData()  // CD for second ware D
                  .setConcreteClassParameter<CompoundWareD>()
                  .setParameter<CompoundWareD::NameD>("D2"))
          .addSubobject<CompoundWareE::WareDInstanceList>(
              ConstructionData()  // CD for third ware D
                  .setConcreteClassParameter<CompoundWareD>()
                  .setParameter<CompoundWareD::NameD>("D3"))
          .addSubobject<CompoundWareBIntermediate::WareBInstance>(
              ConstructionData()  // CD B in Bintermediate
                  .setConcreteClassParameter<CompoundWareB>()
                  .setParameter<CompoundWareB::RealB>(7.77)
                  .addSharedWare(CompoundWareB::WareAPtrRef(
                      "wareAPtrRef",
                      std::make_shared<CompoundWareA>(
                          ConstructionData().setParameter<CompoundWareA::LabelA>(4))))));

  // CompoundWareE consists of:
  // - derived from CompoundWareBIntermediate and CompoundWareBIntermediate
  //   has a memeber variable CompundWareB
  // - CompoundWareB has a shared reference to a CompundWareA object.
  // - un undefined number of CompundWareD objects stored in vector
  ASSERT_EQ(wareE.dList.size(), 3);
  ASSERT_EQ(
      wareE.descr.value(),
      "CompundWareE: testing heterogenous subobjects and subobject list with undefined lenght.");
  ASSERT_EQ(wareE.dList.front()->name.value(), "D1");
  ASSERT_EQ(wareE.dList.at(1)->name.value(), "D2");
  ASSERT_EQ(wareE.dList.back()->name.value(), "D3");
  ASSERT_EQ(wareE.wareB.rB, 7.77);
  ASSERT_EQ(wareE.wareB.wareARef->lA.value(), 4);
}

TEST_F(ConstructionValidatorTests, validateSubObjectForBaseClass)  // funktioniert
{
  // build a CompundWareE object
  auto wareF = CompoundWareF(
      ConstructionData()  // CD ware E
          .setParameter<CompoundWareE::Description>(
              "CompundWareF: derived from CompundWareBintermediate")
          .setParameter<CompoundWareBIntermediate::RealBIntermediate>(44.44)
          .addSubobject<CompoundWareBIntermediate::WareBInstance>(
              ConstructionData()  // CD B in Bintermediate
                  .setConcreteClassParameter<CompoundWareB>()
                  .setParameter<CompoundWareB::RealB>(7.77)
                  .addSharedWare(CompoundWareB::WareAPtrRef(
                      "wareAPtrRef",
                      std::make_shared<CompoundWareA>(
                          ConstructionData().setParameter<CompoundWareA::LabelA>(4))))));

  EXPECT_EQ(wareF.descr.value(), "CompundWareF: derived from CompundWareBintermediate");
  EXPECT_EQ(wareF.rB, 44.44);
  EXPECT_EQ(wareF.wareB.rB, 7.77);
  EXPECT_EQ(wareF.wareB.wareARef->lA, 4);
}

TEST_F(ConstructionValidatorTests, validateConstructionCompoundWareB)  // funktioniert
{
  // build a CompundWareE object
  auto wareB
      = CompoundWareB(ConstructionData().setParameter<CompoundWareB::RealB>(7.77).addSharedWare(
          CompoundWareB::WareAPtrRef(
              "wareAPtrRef", std::make_shared<CompoundWareA>(
                                 ConstructionData().setParameter<CompoundWareA::LabelA>(4)))));

  EXPECT_EQ(wareB.rB, 7.77);
  EXPECT_EQ(wareB.wareARef->lA, 4);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Unit test for testing all check fucntions

TEST_F(ConstructionValidatorTests, checkNamedEnum)
{
  SettingRule sd = SettingRule::forNamedEnum<States>(SettingRule::Usage::MANDATORY_NO_DEFAULT,
                                                     "a container for american states");
  ConstructionValidator cv({sd});
  ConstructionData cd = ConstructionData().setEnum(States::TEXAS);

  ASSERT_TRUE(cv.checkNamedEnum<States>(cd).empty());
  ASSERT_FALSE(cv.checkNamedEnum<States>(ConstructionData()).empty());
}

TEST_F(ConstructionValidatorTests, checkNamedParameterForLabel)
{
  SettingRule sd = SettingRule::forNamedParameter<LabelParam>(SettingRule::Usage::OPTIONAL,
                                                              "a label parameter");
  sd.defaultValue = 15;
  sd.minimalValue = 10;
  sd.maximalValue = 20;
  ConstructionValidator cv({sd});
  LabelParam lp = 17;
  ConstructionData cd = ConstructionData().setParameter(lp);
  ASSERT_TRUE(cv.checkNamedParameter<LabelParam>(cd).empty());
  lp = 5;
  cd.s.setParameter(lp);
  ASSERT_FALSE(cv.checkNamedParameter<LabelParam>(cd).empty());
}

TEST_F(ConstructionValidatorTests, checkNamedParameteForReal)
{
  SettingRule sd
      = SettingRule::forNamedParameter<RealParam>(SettingRule::Usage::OPTIONAL, "a real parameter");
  sd.defaultValue = -50.0;
  sd.minimalValue = -100.0;
  sd.maximalValue = -10.0;
  ConstructionValidator cv({sd});
  RealParam rp = -42.3;
  ConstructionData cd = ConstructionData().setParameter(rp);
  ASSERT_TRUE(cv.checkNamedParameter<RealParam>(cd).empty());
  rp = 0.0;
  cd.s.setParameter(rp);
  ASSERT_FALSE(cv.checkNamedParameter<RealParam>(cd).empty());
}

TEST_F(ConstructionValidatorTests, checkNamedParameterForString)
{
  SettingRule sd = SettingRule::forNamedParameter<StringParam>(SettingRule::Usage::OPTIONAL,
                                                               "a string parameter");
  sd.defaultValue = "bla";
  sd.minimalStringLength = 2;
  ConstructionValidator cv({sd});
  StringParam sp("xx");
  ASSERT_TRUE(cv.checkNamedParameter<StringParam>(ConstructionData().setParameter(sp)).empty());
  sp = "y";
  EXPECT_THAT(
      cv.checkNamedParameter<StringParam>(ConstructionData().setParameter(sp)),
      testing::HasSubstr(
          "Setting for key 'StringParam' and value: 'y' requires a min string length of 2."));
}

TEST_F(ConstructionValidatorTests, checkSharedWareMissingRuleReturnsError)
{
  ConstructionData cdB;
  auto errors = CompoundWareB::getConstructionValidator().checkSharedWare<TrivialReference>(cdB);
  EXPECT_THAT(errors,
              testing::HasSubstr("There is no registered warelistrule for key 'TrivialReference'. "
                                 "Unable to validate the shared ware."));
}

TEST_F(ConstructionValidatorTests, checkSharedListRuleIsforSharedWareReturnsError)
{
  ConstructionData cdB;
  auto errors
      = CompoundWareB::getConstructionValidator().checkSharedWareList<CompoundWareB::WareAPtrRef>(
          cdB);
  EXPECT_THAT(
      errors,
      testing::HasSubstr(
          "Warelistrule for key 'WareAPtrRef' refers to a single shared ware and not a list. Call "
          "'checkSharedWareAndReturnErrors' instead."));
}

TEST_F(ConstructionValidatorTests, checkWithSettingAndWareListRules)
{
  // clang-format off
    ConstructionData cdA = ConstructionData()
                            .setParameter<CompoundWareA::LabelA>(177);

    ConstructionData cdB = ConstructionData()
                            .setParameter<CompoundWareB::RealB>(177.7)
                            .addSharedWare(CompoundWareB::WareAPtrRef("A", std::make_shared<CompoundWareA>(cdA)));

    ConstructionData cdC = ConstructionData()
                               .setEnum(CompoundWareC::ChoiceC::THE_BAD)
                               .addSharedWare(CompoundWareC::WareAFixedListRef("A0", std::make_shared<CompoundWareA>(cdA)))
                               .addSharedWare(CompoundWareC::WareAFixedListRef("A1", std::make_shared<CompoundWareA>(cdA)))
                               .addSubobject<CompoundWareC::WareAFixedListInstance>(cdA)
                               .addSubobject<CompoundWareC::WareAFixedListInstance>(cdA)
                               .addSubobject<CompoundWareC::WareBInstance>(cdB);

  // clang-format on
  ASSERT_EQ("", CompoundWareC::getConstructionValidator().check(cdC));
}

TEST_F(ConstructionValidatorTests, checkForConcreteClass)
{
  ConstructionValidator cv = CompoundWareA::getConstructionValidator();
  ConstructionData cdA;

  EXPECT_THAT(
      cv.check(cdA),
      testing::HasSubstr(
          "SettingRule for key 'LabelA' requires a value offered by consruction data and does not "
          "support setting a default value."));

  cdA.setParameter<CompoundWareA::LabelA>(34);

  ASSERT_TRUE(cv.check(cdA).empty());
}

TEST_F(ConstructionValidatorTests, checkWithGivenSettingRules)
{
  SettingRule sd1 = SettingRule::forNamedEnum<Fruits>(SettingRule::Usage::MANDATORY_NO_DEFAULT,
                                                      "a container for fruits");

  SettingRule sd2 = SettingRule::forNamedParameter<LabelParam>(SettingRule::Usage::OPTIONAL,
                                                               "a label parameter");
  sd2.defaultValue = 15;
  sd2.minimalValue = 10;
  sd2.maximalValue = 20;
  ConstructionValidator cv({sd1, sd2});

  ConstructionData cd;

  ASSERT_FALSE(cv.check((ConstructionData())).empty());
  cd.s = Settings().setParameter(LabelParam(16)).setEnum(Fruits::BANANA);
  ASSERT_TRUE(cv.check(cd).empty());
  cd.s = Settings().setParameter(LabelParam(14)).setEnum(Fruits::ORANGE);
  ASSERT_TRUE(cv.check(cd).empty());
  cd.s = Settings().setParameter(LabelParam(21)).setEnum(Fruits::BANANA);
  ASSERT_FALSE(cv.check(cd).empty());
  cd.s = Settings().setParameter(LabelParam(14));
  ASSERT_FALSE(cv.check(cd).empty());
  cd.s = Settings().setEnum(Fruits::ORANGE);
  ASSERT_TRUE(cv.check(cd).empty());
}

TEST_F(ConstructionValidatorTests, checkSubobject)
{
  auto errors
      = CompoundWareC::getConstructionValidator().checkSubobject<CompoundWareB::WareAPtrRef>(
          ConstructionData());
  EXPECT_THAT(errors,
              testing::HasSubstr("There is no registered warelistrule for key 'WareAPtrRef'. It is "
                                 "not possible to validate the subobject."));

  errors = CompoundWareC::getConstructionValidator()
               .checkSubobject<CompoundWareC::WareAFixedListInstance>(ConstructionData());
  EXPECT_THAT(
      errors,
      testing::HasSubstr(
          "Warelistrule for key 'WareAFixedListInstance' refers to a subobject list and not for a "
          "single subobject. Call 'checkSubobjectListAndReturnErrors' instead."));

  errors = CompoundWareC::getConstructionValidator().checkSubobject<CompoundWareC::WareBInstance>(
      ConstructionData());
  EXPECT_THAT(
      errors,
      testing::HasSubstr("Subobject data for 'WareBInstance' was not passed in construction data"));

  ConstructionData cdB = ConstructionData().addSharedWare(CompoundWareB::WareAPtrRef("A"));
  ConstructionData cdC = ConstructionData().addSubobject<CompoundWareC::WareBInstance>(cdB);

  errors
      = CompoundWareC::getConstructionValidator().checkSubobject<CompoundWareC::WareBInstance>(cdC);
  EXPECT_THAT(errors,
              testing::HasSubstr("In subobject data for 'WareBInstance':\n  ware rule for "
                                 "'WareAPtrRef': was passed empty ware "
                                 "pointer with ware key 'WareAPtrRef' in the construction data"));

  ConstructionData cdA = ConstructionData().setParameter<CompoundWareA::LabelA>(177);
  cdB = ConstructionData().addSharedWare(
      CompoundWareB::WareAPtrRef("A", std::make_shared<CompoundWareA>(cdA)));
  cdC = ConstructionData().addSubobject<CompoundWareC::WareBInstance>(cdB);

  errors
      = CompoundWareC::getConstructionValidator().checkSubobject<CompoundWareC::WareBInstance>(cdC);
  ASSERT_TRUE(errors.empty());
}

TEST_F(ConstructionValidatorTests, checkSubobjectList)
{
  auto errors
      = CompoundWareC::getConstructionValidator().ckeckSubobjectList<CompoundWareB::WareAPtrRef>(
          ConstructionData());
  EXPECT_THAT(errors, testing::HasSubstr("There is no registered warelistrule for key "
                                         "'WareAPtrRef'. Unable to validate the subobjects."));

  errors
      = CompoundWareC::getConstructionValidator().ckeckSubobjectList<CompoundWareC::WareBInstance>(
          ConstructionData());
  EXPECT_THAT(
      errors,
      testing::HasSubstr(
          "Warelistrule for key 'WareBInstance' refers to a single subobject and not a list. Call "
          "'checkSubobjectAndReturnErrors' instead."));

  ConstructionData cdA = ConstructionData().setParameter<CompoundWareA::LabelA>(177);
  ConstructionData cdC
      = ConstructionData().addSubobject<CompoundWareC::WareAFixedListInstance>(cdA);

  errors = CompoundWareC::getConstructionValidator()
               .ckeckSubobjectList<CompoundWareC::WareAFixedListInstance>(cdC);
  EXPECT_THAT(errors, testing::HasSubstr(
                          "There are not enough subobject CDs for keyWareAFixedListInstance"));

  ConstructionData cdC_brokenA = cdC;
  cdC_brokenA.addSubobject<CompoundWareC::WareAFixedListInstance>(ConstructionData());

  errors = CompoundWareC::getConstructionValidator()
               .ckeckSubobjectList<CompoundWareC::WareAFixedListInstance>(cdC_brokenA);
  EXPECT_THAT(
      errors,
      testing::HasSubstr(
          "In subobject data for 'WareAFixedListInstance':\n  SettingRule for key 'LabelA' "
          "requires a "
          "value offered by consruction data and does not support setting a default value."));

  cdC.addSubobject<CompoundWareC::WareAFixedListInstance>(cdA);
  errors = CompoundWareC::getConstructionValidator()
               .ckeckSubobjectList<CompoundWareC::WareAFixedListInstance>(cdC);
  ASSERT_EQ("", errors);
}

TEST_F(ConstructionValidatorTests, checkForInterfaceClass)
{
  ConstructionValidator cv = Base::getConstructionValidator();
  ConstructionData cdBase;

  EXPECT_THAT(cv.check(cdBase),
              testing::HasSubstr(
                  "Subobject data for 'BaseWareAInstance' was not passed in construction data"));

  cdBase = ConstructionData()
               .addSharedWare(Base::BaseWareAReference(
                   "baseWareA", std::make_shared<CompoundWareA>(
                                    ConstructionData().setParameter<CompoundWareA::LabelA>(42))))
               .addSubobject<Base::BaseWareAInstance>(
                   ConstructionData().setParameter<CompoundWareA::LabelA>(43));

  ASSERT_TRUE(cv.check(cdBase).empty());
}

TEST_F(ConstructionValidatorTests, checkSharedListNErrorsReturnsEmptyString)
{
  ConstructionData cdA0 = ConstructionData().setParameter<CompoundWareA::LabelA>(177);
  ConstructionData cdC = ConstructionData()
                             .addSharedWare(CompoundWareC::WareAFixedListRef(
                                 "A0", std::make_shared<CompoundWareA>(cdA0)))
                             .addSharedWare(CompoundWareC::WareAFixedListRef(
                                 "A1", std::make_shared<CompoundWareA>(cdA0)));

  auto errors = CompoundWareC::getConstructionValidator()
                    .checkSharedWareList<CompoundWareC::WareAFixedListRef>(cdC);
  ASSERT_TRUE(errors.empty());
}

TEST_F(ConstructionValidatorTests,
       checkSharedListWarePointerTypeIsNotDerivedFromInterfaceReturnsError)
{
  auto wlr = WarelistRule::forSharedWareList<BaseReference>("a list of Base");
  ConstructionValidator cv({}, {wlr});

  ConstructionData cdBase;
  auto newKey = cdBase.createSharedWareKeyWithCounter(BaseReference::getReferenceName(), nullptr);
  cdBase.sharedWares[newKey] = std::make_shared<const TrivialWare>();
  auto errors = cv.checkSharedWareList<BaseReference>(cdBase);
  EXPECT_THAT(errors, testing::HasSubstr(
                          "ware list rule for 'BaseReference': type mismatch: object "
                          "'BaseReference;0' should be an "
                          "instance of a class derived from '"
                          + FactoryInterface<Base>::getInterfaceName()
                          + "' according to rule, but was passed an object of incorrect type '"
                          + TrivialWare::getClassName() + "' in the construction data"));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! Unit test for testing all validation functions

TEST_F(ConstructionValidatorTests, validateNamedEnum)
{
  ConstructionValidator cv(
      {SettingRule::forNamedEnum<Fruits>(SettingRule::Usage::OPTIONAL, "a container for fruits")});
  ConstructionData cd = ConstructionData().setEnum(Fruits::ORANGE);
  ASSERT_EQ(Fruits::ORANGE, cv.validateNamedEnum<Fruits>(cd).value());
}

TEST_F(ConstructionValidatorTests, validateNamedParameterForLabel)
{
  ConstructionValidator cv({SettingRule::forNamedParameter<LabelParam>(SettingRule::Usage::OPTIONAL,
                                                                       "a label parameter")});
  LabelParam lp = 17;
  ConstructionData cd = ConstructionData().setParameter(lp);
  ASSERT_EQ(17, cv.validateNamedParameter<LabelParam>(cd).value());
}

TEST_F(ConstructionValidatorTests, validateNamedParameterForReal)
{
  ConstructionValidator cv({SettingRule::forNamedParameter<RealParam>(SettingRule::Usage::OPTIONAL,
                                                                      "a real parameter")});
  RealParam rp = -42.3;
  ConstructionData cd = ConstructionData().setParameter(rp);
  ASSERT_EQ(-42.3, cv.validateNamedParameter<RealParam>(cd).value());
}

TEST_F(ConstructionValidatorTests, validateNamedParameterForStringWorks)
{
  SettingRule sr = SettingRule::forNamedParameter<StringParam>(SettingRule::Usage::OPTIONAL,
                                                               "a string parameter");
  sr.minimalStringLength = 5;
  sr.defaultValue = "12345";
  ConstructionValidator cv({sr});
  StringParam sp("abcdef");

  ConstructionData cd = ConstructionData().setParameter(sp);
  ASSERT_EQ(std::string("abcdef"), cv.validateNamedParameter<StringParam>(cd).value());
  ASSERT_EQ(std::string("12345"),
            cv.validateNamedParameter<StringParam>(ConstructionData()).value());
}

TEST_F(ConstructionValidatorTests, validateSharedWareWithReferenceToInterface)
{
  auto wlr = WarelistRule::forSharedWare<BaseReference>("a reference to base");
  ConstructionValidator cv({}, {wlr});

  ConstructionData cdDerived = ConstructionData().setParameter<DerivedA::DerivedAInt>(421);
  ConstructionData cdBase = ConstructionData().addSharedWare(
      BaseReference("derivedA0", std::make_shared<DerivedA>(cdDerived)));

  auto pA = cv.validateSharedWare<BaseReference>(cdBase);
  ASSERT_TRUE(bool(pA.ptr()));
  ASSERT_EQ(421, std::static_pointer_cast<const DerivedA>(BaseReference::Pointer(pA))->id);
}

TEST_F(ConstructionValidatorTests, validateNamedReferenceWithReferenceToInterface)
{
  auto wlr = WarelistRule::forSharedWare<BaseReference>("a reference to base");
  ConstructionValidator cv({}, {wlr});

  ConstructionData cdDerived = ConstructionData().setParameter<DerivedA::DerivedAInt>(421);
  ConstructionData cdBase = ConstructionData().addSharedWare(
      BaseReference("derivedA0", std::make_shared<DerivedA>(cdDerived)));

  ASSERT_EQ("derivedA0", cv.validateNamedReference<BaseReference>(cdBase).getId());
}

TEST_F(ConstructionValidatorTests, validateSharedWareWithValidObject)
{
  ConstructionData cdA = ConstructionData().setParameter<CompoundWareA::LabelA>(-42);
  ConstructionData cdB = ConstructionData().addSharedWare(
      CompoundWareB::WareAPtrRef("wareAPtrRef", std::make_shared<CompoundWareA>(cdA)));
  auto wareA
      = CompoundWareB::getConstructionValidator().validateSharedWare<CompoundWareB::WareAPtrRef>(
          cdB);
  ASSERT_TRUE(bool(wareA.ptr()));
  ASSERT_EQ(-42, wareA->lA);
}

TEST_F(ConstructionValidatorTests, validateSharedWareWithOptionalUsage)
{
  auto modifiedCV = ConstructionValidator(
      {}, {[]() {
        auto wlr
            = WarelistRule::forSharedWare<CompoundWareB::WareAPtrRef>("pointer to const ware A");
        wlr.usage = WarelistRule::Usage::OPTIONAL;
        return wlr;
      }()},
      CompoundWareB::getConstructionValidator());
  ConstructionData cdB;
  ASSERT_EQ("", modifiedCV.checkSharedWare<CompoundWareB::WareAPtrRef>(cdB));
  auto wareA = modifiedCV.validateSharedWare<CompoundWareB::WareAPtrRef>(cdB);
  ASSERT_FALSE(bool(wareA.ptr()));

  auto cdA = ConstructionData().setParameter<CompoundWareA::LabelA>(-42);
  cdB = ConstructionData().addSharedWare(
      CompoundWareB::WareAPtrRef("wareAPtrRef", std::make_shared<CompoundWareA>(cdA)));
  ASSERT_EQ("", modifiedCV.checkSharedWare<CompoundWareB::WareAPtrRef>(cdB));
  wareA = modifiedCV.validateSharedWare<CompoundWareB::WareAPtrRef>(cdB);
  ASSERT_TRUE(bool(wareA.ptr()));
  ASSERT_EQ(-42, wareA->lA);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
