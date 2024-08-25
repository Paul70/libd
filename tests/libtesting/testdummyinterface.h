#ifndef DUTIL_TESTDUMMYINTERFACE_H
#define DUTIL_TESTDUMMYINTERFACE_H
#include "libd/libdutil/projectware.h"

namespace LIBD {
namespace TESTS {

class TestDummyInterface : public DUTIL::ProjectWare
{};
} // namespace TESTS
} // namespace LIBD

D_DECLARE_FACTORYINTERFACE(::LIBD::TESTS::TestDummyInterface)

#define D_DECLARE_TESTDUMMYINTERFACE(REGISTERED_CLASS) \
    static const DUTIL::ConcreteFactory<REGISTERED_CLASS, LIBD::TESTS::TestDummyInterface, DUTIL::ProjectWare> factory;

#define D_DEFINE_TESTDUMMYINTERFACE(REGISTERED_CLASS) \
    const DUTIL::ConcreteFactory<REGISTERED_CLASS, LIBD::TESTS::TestDummyInterface, DUTIL::ProjectWare> REGISTERED_CLASS::factory;

#endif // DUTIL_TESTDUMMYINTERFACE_H
