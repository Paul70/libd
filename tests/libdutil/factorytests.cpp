#include <type_traits>
#include "libd/libdutil/constructiondata.h"
#include "libd/libdutil/factory.h"
#include "libd/libdutil/factoryinterface.h"
#include "libd/libdutil/projectware.h"
#include "libdglobals/globals.h"
#include "tests/libtesting/testdummy.h"
#include "tests/testbase.h"

using namespace LIBD::TESTS;
using namespace DUTIL;

namespace {
class FactoryTests : public TestBase
{};

class ConcreteWareA : public ProjectWare, public D_NAMED_CLASS(::ConcreteWareA)
{
  public:
  // static concrete facotry memeber
  D_DECLARE_PROJECTWARE(ConcreteWareA)

  static ConstructionValidator const& getConstructionValidator()
  {
    static ConstructionValidator cv;
    return cv;
  }

  explicit ConcreteWareA([[maybe_unused]] ConstructionData const& cd) {}

  bool doNothingAndReturnTrue() const { return true; }
};

class ConcreteWareB : public ProjectWare, public D_NAMED_CLASS(::ConcreteWareB)
{
  public:
  // static concrete facotry memeber
  [[maybe_unused]] D_DECLARE_PROJECTWARE(ConcreteWareB)

      static ConstructionValidator const& getConstructionValidator()
  {
    static ConstructionValidator cv;
    return cv;
  }

  explicit ConcreteWareB([[maybe_unused]] ConstructionData const& cd) {}
};

D_DEFINE_PROJECTWARE(ConcreteWareA)
D_DEFINE_PROJECTWARE(ConcreteWareB)

}  // end of anonymous namespace

TEST_F(FactoryTests, testGetConcreteClasses)
{
  // function delivers a Stringlist of all registered, concrete class types
  auto size = TestBase::getNumberOfRegisteredConcreteClasses();
  auto classes = Factory::getConcreteClasses();
  ASSERT_TRUE(classes.size() == static_cast<unsigned long>(size));
}

TEST_F(FactoryTests, testRegisterConcreteClassesAndInterfacesWorksAsExpected)
{
  auto result = Factory::isRegisteredConcreteClassName(TestDummy::getClassName());
  EXPECT_TRUE(result);
  result = Factory::isRegisteredInterface(FactoryInterface<ProjectWare>::getInterfaceName());
  EXPECT_TRUE(result);
  result = Factory::isRegisteredWithInterface(FactoryInterface<ProjectWare>::getInterfaceName(),
                                              TestDummy::getClassName());
  EXPECT_TRUE(result);
}

TEST_F(FactoryTests, testGetConcreteClassesAndInterfacesWorksAsExpected)
{
  auto classes = Factory::getConcreteClasses();
  auto size = TestBase::getNumberOfRegisteredConcreteClasses();
  EXPECT_TRUE(classes.size() == size);
  auto dummy = std::find(classes.cbegin(), classes.cend(), TestDummy::getClassName());
  EXPECT_EQ(TestDummy::getClassName(), *dummy);
  auto interfaces = Factory::getInterfaces();
  auto it = interfaces.find(FactoryInterface<ProjectWare>::getInterfaceName());
  EXPECT_TRUE(it != interfaces.end());
}

TEST_F(FactoryTests, testGetFactoryByName)
{
  Factory const* f = &Factory::getFactoryByName(TestDummy::getClassName());
  // clang-format off
    std::string result = f->checkCD(ConstructionData()
                                    .setConcreteClassParameter<TestDummy>()
                                    .setEnum<TestDummy::COLOR>(TestDummy::COLOR::GREEN));
  // clang-format on
  ASSERT_TRUE(result.empty());
  bool type = std::is_same_v<decltype(f), Factory const*>;
  EXPECT_TRUE(type);
}

TEST_F(FactoryTests, testGetInterfaces)
{
  auto size = TestBase::getNumberOfRegisteredInterfaces();
  auto IF = Factory::getInterfaces();
  ASSERT_TRUE(size == IF.size());
  auto it = IF.find(FactoryInterface<ProjectWare>::getInterfaceName());
  EXPECT_TRUE(it != IF.end());
}

TEST_F(FactoryTests, testGetConcreteClassesForInterface)
{
  auto map = DGLOBALS::getInterfaceMap();
  auto list
      = Factory::getConcreteClassesForInterface(FactoryInterface<ProjectWare>::getInterfaceName());
  ASSERT_TRUE(
      list.size()
      == getNumberOfConcreteClassesForInterface(FactoryInterface<ProjectWare>::getInterfaceName()));
  auto it = std::find(list.cbegin(), list.cend(), TestDummy::getClassName());
  ASSERT_NE(it, list.end());
  it = std::find(list.cbegin(), list.cend(), ConcreteWareA::getClassName());
  ASSERT_NE(it, list.end());
}

TEST_F(FactoryTests, testCreateNewConcreteClassViaTypeSetting)
{
  // ConstructionData with Type setting
  auto newPW = FactoryInterface<DUTIL::ProjectWare>::newInstanceViaTypeSetting(
      ConstructionData().setConcreteClassParameter<ConcreteWareA>());
  bool type = std::is_same_v<decltype(newPW), std::unique_ptr<ProjectWare>>;
  ASSERT_TRUE(type);

  auto newCwA = static_unique_ptr_cast<ConcreteWareA, ProjectWare>(std::move(newPW));
  ASSERT_EQ(ConcreteWareA::getShortClassName(), newCwA->getShortConcreteClassName());
  ASSERT_TRUE(newCwA->doNothingAndReturnTrue());
  type = std::is_same_v<decltype(newCwA), std::unique_ptr<ConcreteWareA>>;
  ASSERT_TRUE(type);

  // same test for ConcreteWareB
  newPW = FactoryInterface<DUTIL::ProjectWare>::newInstanceViaTypeSetting(
      ConstructionData().setConcreteClassParameter<ConcreteWareB>());
  type = std::is_same_v<decltype(newPW), std::unique_ptr<ProjectWare>>;
  ASSERT_TRUE(type);

  auto newCwB = static_unique_ptr_cast<ConcreteWareB, ProjectWare>(std::move(newPW));
  ASSERT_EQ(ConcreteWareB::getShortClassName(), newCwB->getShortConcreteClassName());
  type = std::is_same_v<decltype(newCwB), std::unique_ptr<ConcreteWareB>>;
  ASSERT_TRUE(type);
}

TEST_F(FactoryTests, testCheckDuringFactoryConstructionIsOK)
{
  // clang-format off
    // test is using TestDummy class
    auto newPW = FactoryInterface<DUTIL::ProjectWare>::newInstanceViaTypeSetting(
        ConstructionData()
                .setConcreteClassParameter<TestDummy>()
                .setEnum<TestDummy::COLOR>(TestDummy::COLOR::GREEN));
  // clang-format on

  bool type = std::is_same_v<decltype(newPW), std::unique_ptr<ProjectWare>>;
  ASSERT_TRUE(type);

  auto newTD = static_unique_ptr_cast<TestDummy, ProjectWare>(std::move(newPW));
  ASSERT_EQ(newTD->getNamedEnum(), TestDummy::COLOR::GREEN);
}
