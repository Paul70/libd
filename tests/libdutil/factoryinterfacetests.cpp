#include "libd/libdutil/constructiondata.h"
#include "libd/libdutil/factoryinterface.h"
#include "tests/factorytestsexampleclass.h"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class FactoryInterfaceTests : public TestBase
{};

using namespace DUTIL;
using namespace TESTS;
using FI = FactoryInterface<TESTS::Base>;

}  // namespace

TEST_F(FactoryInterfaceTests, getInterfaceNameForInterfaceBase)
{
  ASSERT_EQ("::TESTS::Base", FI::getInterfaceName());
}

TEST_F(FactoryInterfaceTests, getFactoryNameViaTypeSettings_withGoodConstructionData_works)
{
  ConstructionData cd
      = ConstructionData()
            .setParameter<Ware::DUTIL_Ware_Type>(TESTS::DerivedA::getClassName())
            .addSharedWare(TESTS::Base::BaseWareAReference(
                "baseWareA",
                std::make_shared<TESTS::CompoundWareA>(
                    ConstructionData().setParameter<TESTS::CompoundWareA::LabelA>(42))))
            .addSubobject<TESTS::Base::BaseWareAInstance>(
                ConstructionData().setParameter<TESTS::CompoundWareA::LabelA>(43));

  ASSERT_EQ(TESTS::DerivedA::getClassName(),
            DUTIL::FactoryInterfaceDetail::getConcreteClassNameViaWareTypeSetting(cd));
}

TEST_F(FactoryInterfaceTests, newInstanceViaTypeSettings_withGoodConstructionData_works)
{
  ConstructionData cd
      = ConstructionData()
            .setParameter<Ware::DUTIL_Ware_Type>(DerivedA::getClassName())
            .setParameter<DerivedA::DerivedAInt>(42)
            .addSharedWare(Base::BaseWareAReference(
                "baseWareA", std::make_shared<CompoundWareA>(
                                 ConstructionData().setParameter<CompoundWareA::LabelA>(1))))
            .addSubobject<Base::BaseWareAInstance>(
                ConstructionData().setParameter<CompoundWareA::LabelA>(-5));

  ASSERT_TRUE(bool(FI::newInstanceViaTypeSetting(cd)));
}
