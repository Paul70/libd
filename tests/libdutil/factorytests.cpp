#include "libd/libdutil/factory.h"
#include "libd/libdutil/factoryinterface.h"
#include "tests/testbase.h"
#include "tests/testdummy.h"
#include <type_traits>

using namespace LIBD::TESTS;
using namespace DUTIL;

namespace {
class FactoryTests : public TestBase
{};
} // end of anonymous namespace

TEST_F(FactoryTests, testConstructionOfConcreteFactoryWorksAsExpected)
{
    //    TestDummy::COLOR color{TestDummy::COLOR::GREEN};
    //    TestDummy td(color);
    //    EXPECT_TRUE(td.getNamedEnum() == color);
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
    EXPECT_TRUE(classes.size() == 1);
    EXPECT_EQ(TestDummy::getClassName(), classes.front());
    auto interfaces = Factory::getInterfaces();
    auto it = interfaces.find(FactoryInterface<ProjectWare>::getInterfaceName());
    EXPECT_TRUE(it != interfaces.end());
}

TEST_F(FactoryTests, testGetFactoryByNameWorksAsExpected)
{
    //auto const *factory = Factory::getFactoryByName(TestDummy::getClassName());
    //bool type = std::is_same_v<decltype(factory), Factory const>;
    //EXPECT_TRUE(type);
}
