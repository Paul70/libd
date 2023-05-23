#ifndef LIBD_TESTS_TESTDUMMY_H
#define LIBD_TESTS_TESTDUMMY_H
#include "libd/libdutil/namedclass.h"
#include "libd/libdutil/namedenum.h"
#include "libd/libdutil/projectware.h"

namespace DUTIL {
struct ConstructionData;
}

namespace LIBD {
namespace TESTS {

/*! brief description of TestDummy
 *
 * Longer description of TestDummy.
 */

class TestDummy : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::TestDummy)
{
public:
    //! NamedEnum Color for test purposes.
    D_NAMED_ENUM(COLOR, RED = 5, BLUE = 10, GREEN)

    //! Default-construct.
    TestDummy(COLOR color);

    COLOR const& getNamedEnum() const;

private:
    COLOR color_;
};

} // namespace TESTS
} // namespace LIBD

#endif // LIBD_TESTS_TESTDUMMY_H
