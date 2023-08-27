#include "constructiondata.h"
#include <algorithm>

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

std::string ConstructionData::createSharedWareKeyWithCounter(std::string key) const
{
    return key;
}

// A key for a subobject consist of the given input string key plus an optional count value.
//
// That makes it possible to register more then one ConstructionData object all referring to the
// same named reference and using the reference name as key.
std::string ConstructionData::createSubObjectKeyWithCounter(std::string key) const
{
    label_t count = 0;
    std::string prefix = key + seperator;
    std::for_each(subObjectData.cbegin(), subObjectData.cend(), [&](auto const &iter) {
        if (iter.first.substr(0, prefix.size()) == prefix)
            ++count;
    });
    return prefix + Utility::toStr(count);
}

std::map<std::string, ConstructionData>::const_iterator ConstructionData::getSubObjectWithCounter(std::string key,
                                                                                                  label_t index) const
{
    return std::map<std::string, ConstructionData>::const_iterator() = subObjectData.find(key + seperator + Utility::toStr(index));
}

} // namespace DUTIL
