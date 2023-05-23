#ifndef DUTIL_VARIANT_H
#define DUTIL_VARIANT_H
#include "basictypes.h"
#include "exception.h"
#include "namedenum.h"
#include "utility.h"
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace DUTIL {
namespace VariantDetail {
template<typename T>
struct is_allowed_arithmetic_type : public std::disjunction<std::is_same<real_t, T>,
                                                            std::is_same<label_t, T>,
                                                            std::is_same<int64_t, T>,
                                                            std::is_same<uint64_t, T>,
                                                            std::is_same<bool, T>,
                                                            std::is_same<double, T>,
                                                            std::is_same<char, T>>
{};

template<typename T>
constexpr bool is_allowed_arithmetic_type_v = is_allowed_arithmetic_type<T>::value;

template<typename T>
struct is_allowed_type : public std::disjunction<is_allowed_arithmetic_type<T>, Utility::is_string<T>>
{};

template<typename T>
constexpr bool is_allowed_type_v = is_allowed_type<T>::value;

template<typename... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};
template<typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
} // namespace VariantDetail

/*! brief description of Variant
 *
 * Longer description of Variant.
 */

class Variant
{
public:
    // muss ich noch mit einem named enum machen
    D_NAMED_ENUM(TypeNamed, MONO, INT, UINT, DOUB, BOO, CHA, STR)

    enum class Type { MONOSTATE,
                      INT64,
                      UINT64,
                      DOUBLE,
                      BOOL,
                      CHAR,
                      STRING };

    using DutilVariant = std::variant<std::monostate, std::int64_t, std::uint64_t, double, bool, char, std::string>;

    //! Default construct empty variant, current variant type is std::monostate.
    explicit Variant();

    //! constructor with initial value
    template<typename InitialType, std::enable_if_t<VariantDetail::is_allowed_type_v<InitialType>, bool> = true>
    explicit Variant(InitialType const value) :
        var_(value),
        type_()
    {
        if constexpr(std::is_same_v<InitialType, std::int64_t>)
            type_ = Type::INT64;
        else if constexpr(std::is_same_v<InitialType, std::uint64_t>)
            type_ = Type::UINT64;
        else if constexpr(std::is_same_v<InitialType, double>)
            type_ = Type::DOUBLE;
        else if constexpr(std::is_same_v<InitialType, bool>)
            type_ = Type::BOOL;
        else if constexpr(std::is_same_v<InitialType, char>)
            type_ = Type::CHAR;
        else
            type_ = Type::STRING;
    }

    Type getType() const;

    bool isMonostate() const;
    bool isString() const;
    bool isNumeric() const;
    bool isCharacter() const;
    bool isBool() const;

    // returns a copy of the value as the specified type
    template<typename T, std::enable_if_t<VariantDetail::is_allowed_type_v<T>, bool> = true>
    std::pair<bool, T> getAs() const
    {
        std::pair<bool, T> result;
        std::visit(VariantDetail::overloaded{
                       [&result](auto arg) {
                           using VariantType = std::decay_t<decltype(arg)>;
                           if constexpr(std::is_unsigned_v<T> && !std::is_unsigned_v<VariantType>) {
                               if(arg < 0) { // change sign
                                   arg *= -1;
                               }
                           }
                           if constexpr(std::is_arithmetic_v<T>) { // both types T and VariantType are arithmetic
                               result.first = std::is_convertible_v<VariantType, T>;
                               if(result.first) {
                                   result.second = static_cast<T>(arg);
                                   return;
                               }
                               result.second = T(0);
                           }
                           else { // Target type T is string and variant type is arithmetic
                               result = Utility::toString(arg);
                           }
                       },
                       [&result](std::string arg) { // try to convert string into arithmetic type
                           result = Utility::fromString<T>(arg);
                       },
                       [&result](std::monostate) {
                           result.first = false;
                           result.second = T();
                       }},
                   var_);

        return result;
    }

private:
    DutilVariant var_;
    Type type_;
};
} // namespace DUTIL
#endif // DUTIL_VARIANT_H
