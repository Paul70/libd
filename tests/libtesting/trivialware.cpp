#include "trivialware.h"

using namespace TESTS;
using namespace DUTIL;

D_DEFINE_PROJECTWARE(TrivialWare);
D_DEFINE_PROJECTWARE(TrivialWare2);

ConstructionValidator const &TrivialWare::getConstructionValidator()
{
    static const ConstructionValidator cv;
    return cv;
}

TrivialWare::TrivialWare(ConstructionData const &)
{}

ConstructionValidator const &TrivialWare2::getConstructionValidator()
{
    static const ConstructionValidator cv;
    return cv;
}

TrivialWare2::TrivialWare2(ConstructionData const &)
{}
