#ifndef LIBD_TESTS_TESTDUMMY_H
#define LIBD_TESTS_TESTDUMMY_H
#include "libd/libdutil/concretefactory.h"
#include "libd/libdutil/namedclass.h"
#include "libd/libdutil/namedenum.h"
#include "libd/libdutil/projectware.h"

namespace DUTIL {
struct ConstructionData;
}

namespace LIBD {
namespace TESTS {

/*! \brief Concrete class for testing purposes.
 *
 * Class contains and implements libd features.
 * Tested features are:
 *  - NamedClass macro
 *  - NamedEnum macro
 *  - ConcreteFactory macro
 */

class TestDummy : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::TestDummy)
{
public:
    //! Static concrete factroy member.
    D_DECLARE_PROJECTWARE(TestDummy)

    //! NamedEnum Color for test purposes.
    D_NAMED_ENUM(COLOR, RED = 5, BLUE = 10, GREEN)

    //! Default-construct.
    TestDummy(COLOR color);

    COLOR const &getNamedEnum() const;

private:
    COLOR color_;
};

} // namespace TESTS
} // namespace LIBD

#endif // LIBD_TESTS_TESTDUMMY_H
