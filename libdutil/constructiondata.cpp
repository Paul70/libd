#include "constructiondata.h"
#include <algorithm>

namespace DUTIL {

ConstructionData::ConstructionData() :
    usage_(Usage::REAL)
{}

ConstructionData::ConstructionData(Usage u) :
    usage_(u)
{}

bool ConstructionData::isProxy() const
{
  if (usage_ == Usage::PROXY)
    return true;
  else
    return false;
}

bool ConstructionData::hasDataset() const
{
  if (ds.getNumberOfCurrentlyStoredData() == 0)
    return false;
  return true;
}

ConstructionData& ConstructionData::set(Settings const sNew) &
{
  s = sNew;
  return *this;
}

ConstructionData&& ConstructionData::set(Settings sNew) &&
{
  s = sNew;
  return std::move(*this);
}

ConstructionData& ConstructionData::set(Dataset dsNew) &
{
  ds = dsNew;
  return *this;
}

ConstructionData&& ConstructionData::set(Dataset dsNew) &&
{
  ds = dsNew;
  return std::move(*this);
}

// A key for a shared ware consist of the given input string key plus an optional count value.
//
// That makes it possible to register multiple shared ware objects all referring to the
// same named reference and using the reference name as part of the key.
std::string ConstructionData::createSharedWareKeyWithCounter(
    std::string key, std::shared_ptr<const Ware> sharedWarePtr) const
{
  label_t count = 0;
  std::string prefix = key + seperator;
  for (auto const& iter : sharedWares) {
    if (iter.second == sharedWarePtr) {
      // In case, a shared ptr already exists pointing to the same object as
      // input 'sharedWarePtr' does return the already exsiting key and overwrite the sharedWare entry.
      break;
    }
    if (iter.first.substr(0, prefix.size()) == prefix) {
      ++count;
    }
  }
  return (prefix + Utility::toString(count));
}

// A key for a subobject consist of the given input string key plus an optional count value.
//
// That makes it possible to register multible construction data objects all referring to the
// same named reference and using the reference name as part of the key.
std::string ConstructionData::createSubObjectKeyWithCounter(std::string key) const
{
  label_t count = 0;
  std::string prefix = key + seperator;
  std::for_each(subObjectData.cbegin(), subObjectData.cend(), [&](auto const& iter) {
    if (iter.first.substr(0, prefix.size()) == prefix)
      ++count;
  });
  return prefix + Utility::toString(count);
}

std::map<std::string, ConstructionData>::const_iterator ConstructionData::getSubObjectWithCounter(
    std::string key, label_t index) const
{
  return std::map<std::string, ConstructionData>::const_iterator()
         = subObjectData.find(key + seperator + Utility::toString(index));
}

std::map<std::string, std::shared_ptr<const Ware>>::const_iterator
ConstructionData::getSharedWareWithCounter(std::string key, label_t i) const
{
  return std::map<std::string, std::shared_ptr<const Ware>>::const_iterator()
         = sharedWares.find(key + seperator + Utility::toString(i));
}

}  // namespace DUTIL
