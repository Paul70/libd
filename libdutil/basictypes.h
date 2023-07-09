#ifndef DUTIL_BASICTYPES_H
#define DUTIL_BASICTYPES_H
#include <cstdint>
#include <string>
#include <vector>

namespace DUTIL {
namespace BasicTypes {

//! Some type definitions;
using label_t = int;
using real_t = double;
using StringList = std::vector<std::string>;

//! Type trait to identify pure integer types, i.e. int, std::int8_t, std::int16_t, std::int32_t, std::int64_t.
template<typename T>
struct is_integer :
    public std::disjunction<std::is_same<label_t, T>,
                            std::is_same<int, T>,
                            std::is_same<std::int8_t, T>,
                            std::is_same<std::int16_t, T>,
                            std::is_same<std::int32_t, T>,
                            std::is_same<std::int64_t, T>>
{};
template<typename T>
constexpr bool is_integer_v = is_integer<T>::value;

//! Type trait to identify std::string types
template<typename T>
struct is_string :
    public std::disjunction<std::is_same<char *, typename std::decay_t<T>>,
                            std::is_same<char const *, typename std::decay_t<T>>,
                            std::is_same<std::string, typename std::decay_t<T>>>
{};
template<typename T>
constexpr bool is_string_v = is_string<T>::value;

} // namespace BasicTypes
} // namespace DUTIL
#endif // DUTIL_BASICTYPES_H
