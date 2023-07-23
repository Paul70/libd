#include "libd/libdutil/constructiondata.h"
#include "libd/libdutil/factory.h"
#include "libd/libdutil/factoryinterface.h"
#include "libd/libdutil/projectware.h"
#include "libdglobals/globals.h"
#include "tests/testbase.h"
#include "tests/testdummy.h"
#include <type_traits>

using namespace LIBD::TESTS;
using namespace DUTIL;

namespace {
class FactoryTests : public TestBase
{};

class ConcreteWareA : public ProjectWare, public D_NAMED_CLASS(::ConcreteWareA)
{
public:
    // static concrete facotry memeber
    [[maybe_unused]] D_DECLARE_PROJECTWARE(ConcreteWareA)

        static ConstructionValidator const &getConstructionValidator()
    {
        static ConstructionValidator cv;
        return cv;
    }

    explicit ConcreteWareA([[maybe_unused]] ConstructionData const &cd)
    {}
};

class ConcreteWareB : public ProjectWare, public D_NAMED_CLASS(::ConcreteWareB)
{
public:
    // static concrete facotry memeber
    [[maybe_unused]] D_DECLARE_PROJECTWARE(ConcreteWareB)

        static ConstructionValidator const &getConstructionValidator()
    {
        static ConstructionValidator cv;
        return cv;
    }

    explicit ConcreteWareB([[maybe_unused]] ConstructionData const &cd)
    {}
};

D_DEFINE_PROJECTWARE(ConcreteWareA)
D_DEFINE_PROJECTWARE(ConcreteWareB)

} // end of anonymous namespace

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
    result = Factory::isRegisteredWithInterface(FactoryInterface<ProjectWare>::getInterfaceName(), TestDummy::getClassName());
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
    DUTIL::ConstructionData cd;
    Factory const *f = &Factory::getFactoryByName(TestDummy::getClassName());
    std::string result = f->checkCD(cd);
    ASSERT_TRUE(result.empty());
    bool type = std::is_same_v<decltype(f), Factory const *>;
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
    auto list = Factory::getConcreteClassesForInterface(FactoryInterface<ProjectWare>::getInterfaceName());
    ASSERT_TRUE(list.size() == getNumberOfConcreteClassesForInterface(FactoryInterface<ProjectWare>::getInterfaceName()));
    auto it = std::find(list.cbegin(), list.cend(), TestDummy::getClassName());
    ASSERT_NE(it, list.end());
    it = std::find(list.cbegin(), list.cend(), ConcreteWareA::getClassName());
    ASSERT_NE(it, list.end());
}

// was ich noch machen muss ist die Tests mit dem Testdummy so umschreiben,
// dass sie stand alone sind
