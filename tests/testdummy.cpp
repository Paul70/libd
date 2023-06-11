#include "testdummy.h"
#include "libd/libdutil/constructiondata.h"

using namespace LIBD;
using namespace TESTS;
using namespace DUTIL;

D_DEFINE_PROJECTWARE(TestDummy);

TestDummy::TestDummy(COLOR color) : color_(color) {}

TestDummy::COLOR const &TestDummy::getNamedEnum() const
{
    return color_;
}
