#include "testdummy.h"
#include "libd/libdutil/constructiondata.h"

using namespace LIBD::TESTS;
using namespace DUTIL;

TestDummy::TestDummy(COLOR color) :
    color_(color)
{}

TestDummy::COLOR const& TestDummy::getNamedEnum() const
{
    return color_;
}
