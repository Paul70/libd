#include "libdutil/constructiondata.h"
#include "libdutil/namedparameter.h"
#include "libdutil/settings.h"
#include "tests/libtesting/testdummy.h"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class SettingsTests : public TestBase
{};

// define some named enums used in this test suite.
D_NAMED_ENUM(GREETINGS, HELLO, GOODBYE, CIAO, SERVUS);
D_NAMED_ENUM(WEEKDAY, FRIDAY, SATURDAY, SUNDAY);
D_NAMED_ENUM(COLOR, RED = 5, BLUE = 10, GREEN);

D_NAMED_BOOL(BoolWithName)
D_NAMED_STRING(StringWithName)
D_NAMED_LABEL(LabelWithName)
D_NAMED_REAL(RealWithName)

}  // namespace

TEST_F(SettingsTests, testConstructionOfSettingsObjectWorksAsExpected)
{
  Settings s{};
  EXPECT_TRUE(s.empty());
  auto keys = s.keys();
  EXPECT_TRUE(keys.empty());
}

TEST_F(SettingsTests, testHasKeyWorksAsExpected)
{
  Settings s{};
  EXPECT_FALSE(s.hasKey("Hallo"));
  s.setEnum(WEEKDAY::FRIDAY);
  EXPECT_TRUE(s.hasKey("WEEKDAY"));
}

TEST_F(SettingsTests, testSetEnumWorksAsExpected)
{
  Settings s = Settings().setEnum(WEEKDAY::FRIDAY);
  EXPECT_TRUE(s.hasKey("WEEKDAY"));
  WEEKDAY wd{WEEKDAY::FRIDAY};
  Settings s2 = Settings().setEnum(wd);
  EXPECT_TRUE(s.hasKey("WEEKDAY"));

  auto value = s.value("WEEKDAY");
  EXPECT_EQ(WEEKDAY(value), wd);
}

TEST_F(SettingsTests, testGetEnumWorksAsExpected)
{
  Settings s = Settings().setEnum(WEEKDAY::FRIDAY);
  WEEKDAY wd{WEEKDAY::FRIDAY};

  auto settingsEnum = s.getEnum<WEEKDAY>();
  ASSERT_EQ(wd, settingsEnum);
  ASSERT_EQ(wd.getEnumName(), settingsEnum.getEnumName());
  ASSERT_EQ(WEEKDAY::FRIDAY, settingsEnum);
}

TEST_F(SettingsTests, testHasKeysWorksAsExpected)
{
  Settings s = Settings().setEnum(COLOR::GREEN);
  EXPECT_TRUE(s.hasKey("COLOR"));
  WEEKDAY wd = WEEKDAY::SUNDAY;
  s.setEnum(wd);
  EXPECT_TRUE(s.hasKey("WEEKDAY"));

  auto keys = s.keys();
  EXPECT_TRUE(keys.size() == 2);
  EXPECT_TRUE(keys[0] == "COLOR");
  EXPECT_TRUE(keys[1] == "WEEKDAY");

  s.set("Integer", Variant(123));
  s.set("Double", Variant(123.456));
  s.set("String", Variant("Hello World"));
  keys = s.keys();
  EXPECT_TRUE(keys.size() == 5);
  EXPECT_TRUE(keys[2] == "Integer");
  EXPECT_TRUE(keys[3] == "Double");
  EXPECT_TRUE(keys[4] == "String");
}

TEST_F(SettingsTests, testEraseWorksAsExpected)
{
  Settings s = Settings().setEnum(GREETINGS::SERVUS);
  s.setEnum(COLOR::GREEN);
  s.setEnum(WEEKDAY::SATURDAY);
  auto keys = s.keys();
  EXPECT_TRUE(keys.size() == 3);
  s.erase(COLOR::getEnumName());
  s.erase(WEEKDAY::getEnumName());
  s.erase(GREETINGS::getEnumName());
  keys = s.keys();
  EXPECT_TRUE(keys.size() == 0);
}

TEST_F(SettingsTests, testSetFromVariantWorksAsExpected)
{
  Variant v1 = Variant(GREETINGS::CIAO);
  Variant v2 = Variant("Hello World!");
  Variant v3 = Variant(123.456);
  Variant v4 = Variant(std::int32_t(123456789));
  Settings s = Settings().setFromVariant(GREETINGS::getEnumName(), v1);
  s.setFromVariant("String", v2);
  s.setFromVariant("Double", v3);
  s.setFromVariant("Label_t", v4);
  auto keys = s.keys();
  EXPECT_TRUE(keys.size() == 4);
  ASSERT_EQ(s.value("GREETINGS"), v1);
  ASSERT_EQ(s.value("String"), v2);
  ASSERT_EQ(s.value("Double"), v3);
  ASSERT_EQ(s.value("Label_t"), v4);
}

TEST_F(SettingsTests, testSetParameterWorksAsExpected)
{
  BoolWithName bwn(true);
  Settings s = Settings().setParameter(bwn);
  ASSERT_TRUE(s.hasKey(bwn.getParameterName()));
  Variant var = s.value(bwn.getParameterName());
  ASSERT_TRUE(var.toBool());

  StringWithName swn("Hello_World!");
  s.setParameter(swn);
  ASSERT_TRUE(s.hasKey(swn.getParameterName()));
  var = s.value(swn.getParameterName());
  ASSERT_EQ(var.toString(), swn.value());

  LabelWithName lwn(123456);
  s.setParameter(lwn);
  ASSERT_TRUE(s.hasKey(lwn.getParameterName()));
  var = s.value(lwn.getParameterName());
  ASSERT_EQ(var.toLabel(), lwn.value());

  RealWithName rwn(123.4567);
  s.setParameter(rwn);
  ASSERT_TRUE(s.hasKey(rwn.getParameterName()));
  var = s.value(rwn.getParameterName());
  ASSERT_EQ(var.toReal(), rwn.value());
}

TEST_F(SettingsTests, testGetParameterWorksAsExpected)
{
  BoolWithName bwn(true);
  Settings s = Settings().setParameter(bwn);
  ASSERT_TRUE(s.hasKey(bwn.getParameterName()));
  auto np_bool = s.getParameter<BoolWithName>();
  ASSERT_EQ(bwn.value(), np_bool.value());

  StringWithName swn("Hello_World!");
  s = Settings().setParameter(swn);
  ASSERT_TRUE(s.hasKey(swn.getParameterName()));
  auto np_str = s.getParameter<StringWithName>();
  ASSERT_EQ(swn.value(), np_str.value());

  LabelWithName lwn(123456);
  s = Settings().setParameter(lwn);
  ASSERT_TRUE(s.hasKey(lwn.getParameterName()));
  auto np_label = s.getParameter<LabelWithName>();
  ASSERT_EQ(lwn.value(), np_label.value());

  RealWithName rwn(123.4567);
  s = Settings().setParameter(rwn);
  ASSERT_TRUE(s.hasKey(rwn.getParameterName()));
  auto np_real = s.getParameter<RealWithName>();
  ASSERT_EQ(rwn.value(), np_real.value());
}

TEST_F(SettingsTests, testSetConcreteClassWorksAsExpected)
{
  using namespace LIBD::TESTS;
  ConstructionData cd = ConstructionData().setEnum(TestDummy::COLOR::GREEN);
  TestDummy td(cd);
  Settings s = Settings().setConcreteClassParameter<TestDummy>();
  ASSERT_TRUE(s.hasKey(TestDummy::Ware::DUTIL_Ware_Type::getParameterName()));
  auto ccp = s.getParameter<TestDummy::Ware::DUTIL_Ware_Type>();
  ASSERT_EQ(td.getClassName(), ccp.value());
}
