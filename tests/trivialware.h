#ifndef LIBD_TESTS__TRIVIALWARE_H
#define LIBD_TESTS__TRIVIALWARE_H
#include "libd/libdutil/concretefactory.h"
#include "libd/libdutil/namedclass.h"
#include "libd/libdutil/projectware.h"

namespace TESTS {

class TrivialWare : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TrivialWare)
{
public:
    D_DECLARE_PROJECTWARE(TrivialWare);
    static DUTIL::ConstructionValidator const &getConstructionValidator();
    TrivialWare() = default;
    explicit TrivialWare(DUTIL::ConstructionData const &);
    std::string id;
};

class TrivialWare2 : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TrivialWare2)
{
public:
    D_DECLARE_PROJECTWARE(TrivialWare2);
    static DUTIL::ConstructionValidator const &getConstructionValidator();
    TrivialWare2() = default;
    explicit TrivialWare2(DUTIL::ConstructionData const &);
    std::string id;
};

} // namespace TESTS
#endif // LIBD_TESTS__TRIVIALWARE_H
