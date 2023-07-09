#include "constructiondata.h"

namespace DUTIL {

ConstructionData &ConstructionData::set(Settings const sNew) &
{
    s = sNew;
    return *this;
}

ConstructionData &&ConstructionData::set(Settings sNew) &&
{
    s = sNew;
    return std::move(*this);
}
} // namespace DUTIL
