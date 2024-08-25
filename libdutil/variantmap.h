#ifndef DUTIL_VARIANTMAP_H
#define DUTIL_VARIANTMAP_H

#include <map>
#include <string>
#include <variant>

namespace DUTIL {

/*! \brief description of VariantMap
 *
 * Longer description of VariantMap.
 */

template<typename Key, template<typename> class MapContent, typename ...Ts>
class VariantTemplateMap
{
public:
    template <typename T>
    void add(Key const& key, T value) {
      auto my = MapContent<T>(value);
      myMap.emplace(key, my);
    }

    template <typename T>
    auto& get(std::string key) {
      auto element = myMap.find(key);
      return std::get<MapContent<T>>(element->second);
    }
private:
    std::map<Key, std::variant<MapContent<Ts>...>> myMap;
};

} // namespace DUTIL
#endif // DUTIL_VARIANTMAP_H
