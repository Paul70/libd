#include "settings.h"

namespace DUTIL {

Settings::Settings() {}

bool Settings::empty() const
{
    if (valueMap_.empty())
        return true;
    return false;
}

bool Settings::hasKey(std::string const &key) const
{
    for (auto const &i : valueMap_) {
        if (i.first == key)
            return true;
    }
    return false;
}

StringList Settings::keys() const noexcept
{
    size_t i = 0, size = valueMap_.size();
    StringList list{};
    list.reserve(size);
    for (; i < size; ++i) {
        list.push_back(valueMap_[i].first);
    }
    return list;
}

Variant Settings::value(std::string const key, Variant const &defaultValue) const
{
    if (key.empty())
        return defaultValue;

    size_t i = 0, size = valueMap_.size();
    for (; i < size; ++i) {
        if (valueMap_[i].first == key)
            return valueMap_[i].second;
    }
    return defaultValue;
}

Settings &Settings::setFromVariant(std::string key, DUTIL::Variant variant)
{
    if (key.empty())
        return *this;

    size_t i = 0, size = valueMap_.size();
    for (; i < size; ++i) {
        if (valueMap_[i].first == key) {
            valueMap_[i].second = variant; // override map entry if it already exists.
            break;
        }
    }
    if (i == size)
        valueMap_.emplace_back(std::make_pair(key, variant));
    return *this;
}

Settings &Settings::erase(std::string const key)
{
    if (key.empty())
        return *this;

    auto i = valueMap_.begin();
    while (i != valueMap_.end() && i->first != key) {
        ++i;
    }
    if (i != valueMap_.end())
        valueMap_.erase(i);
    return *this;
}

Settings::MapType const &Settings::get() const
{
    return valueMap_;
}

bool operator==(Settings const &lhs, Settings const &rhs)
{
    auto lhsMap = lhs.get();
    auto rhsMap = rhs.get();
    return (std::tie(lhsMap) == std::tie(rhsMap));
}

bool operator!=(Settings const &lhs, Settings const &rhs)
{
    return !(lhs == rhs);
}
} // namespace DUTIL
