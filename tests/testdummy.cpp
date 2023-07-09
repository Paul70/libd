#include "testdummy.h"
#include "libd/libdutil/constructiondata.h"
#include "libd/libdutil/constructionvalidator.h"

using namespace LIBD;
using namespace TESTS;
using namespace DUTIL;

D_DEFINE_PROJECTWARE(TestDummy);

DUTIL::ConstructionValidator const &TestDummy::getConstructionValidator()
{
    static ConstructionValidator cv = ConstructionValidator();
    return cv;
}

TestDummy::TestDummy(DUTIL::ConstructionData const &cd) :
    color_(cd.s.getEnum<COLOR>())
{}

TestDummy::COLOR const &TestDummy::getNamedEnum() const
{
    return color_;
}
