#ifndef DUTIL_NAMEDENUM_H
#define DUTIL_NAMEDENUM_H
#include "basictypes.h"
#include "utility.h"
#include <map>

namespace DUTIL {
class Variant;

namespace NamedEnumDetail {
//! Helper for calling libdutil throw exception macro.
void dealWithExcpetion(const std::string msg);

template<typename ENUM_BASE_TYPE>
using MapType = std::map<ENUM_BASE_TYPE, std::string>;

/*! \brief Return Variant value as string
 *
 * See also DUTIL::Variant::toString function.
 * If no conversion is possible, function returns an empty string.
 */
std::string variantToString(DUTIL::Variant const &variant);

/*! \brief Fill the map with enum value strings
 *
 * Throws an exception in case of unconvenient string arguments.
 */
template<typename ENUM_BASE_TYPE>
void initNameMap(MapType<ENUM_BASE_TYPE> &map, std::string const &enumName, std::string const &arg)
{
    StringList list = Utility::split(arg);
    ENUM_BASE_TYPE initVal = 0;

    for (auto arg : list) {
        if (arg.find('=') != std::string::npos) {
            auto nameValuePair = Utility::split(arg, '=');
            if (nameValuePair.size() != 2) {
                dealWithExcpetion("Parsing of enum argument list failed.");
            }
            auto value = Utility::fromString<ENUM_BASE_TYPE>(nameValuePair[1]);
            if (!value.first) {
                dealWithExcpetion("Could not convert enum argument into ENUM_BASE_TYPE of '" + enumName + "'.");
            }
            initVal = value.second;
            arg = nameValuePair[0];
        }
        map.emplace(std::make_pair(initVal++, arg));
    }
}

/*! \brief Get a list of all registered enum value strings.
 *
 * This function does not throw exceptions.
 */
template<typename ENUM_BASE_TYPE>
StringList getListOfAllowedValues(MapType<ENUM_BASE_TYPE> const &map) noexcept
{
    if (map.empty()) {
        return StringList{};
    }
    StringList list;
    list.reserve(map.size());
    for (auto const &node : map) {
        list.push_back(node.second);
    }
    return list;
}

/*! \brief Return corresponding base type to given enum string value.
 *
 * Basically, this function searches for an enum string inside the enum value map
 * to initialize an enum type object with the underlying enum base type.
 * If that string is not found, an exception is thrown. In case of an empty input
 * string, the defalut enum value is used as return value.
 */
template<typename ENUM_BASE_TYPE>
ENUM_BASE_TYPE fromStringToBaseType(MapType<ENUM_BASE_TYPE> const &map, ENUM_BASE_TYPE defaultValue, std::string const &input)
{
    std::string str = Utility::trim(input);
    if (str.empty()) {
        return defaultValue;
    }

    // look up string in map
    for (auto const &node : map) {
        if (node.second == str) {
            return node.first;
        }
    }

    // string not found
    std::string allowedValues;
    for (auto const &node : map) {
        allowedValues += node.second + ",";
    }
    NamedEnumDetail::dealWithExcpetion("The input string '" + input
                                       + "' is not registered as an enum value. Allowed values are:" + allowedValues);
    return ENUM_BASE_TYPE();
}

} // namespace NamedEnumDetail

/*! \brief NamedEnum, a smart enum knowing its type name.
 *
 * This enum class offers a type-to-string realtion and vice versa, meaning that you can crate enum type values
 * from string parameters. Knowing the name of an enum type as a string lets you create enum values
 * of this type during runtime.
 *
 * Use the macro given below to declare such smart enum types.
 * NamedEnum class is desigend after the curiously recurring template pattern (CRTP) which makes it possible to
 * use methods of the concrete named enum type inside the named enum base class. More precisely, CRTP allows us
 * to transfer all concrete enum values (the __VA_ARGS__ of the macro) into the base class where all the type
 * to string logic happens.
 */

template<typename DERIVED_ENUM_CLASS, typename ENUM_BASE_TYPE>
class NamedEnumBase
{
public:
    //! Check if enum base type is allowed and offers required, arithmetic operations.
    static_assert(std::is_arithmetic_v<ENUM_BASE_TYPE>, "Enum Base Type has to be an arithmetic type.");

    //! Default construct named enum object. Default value is the first enum given in the list.
    explicit NamedEnumBase() :
        value_(NamedEnumBase::getDefaultValue())
    {}

    //! Construct from ENUM_BASE_TYPE object.
    explicit NamedEnumBase(ENUM_BASE_TYPE value) :
        value_(value)
    {
        if (!isValidValue(value)) {
            NamedEnumDetail::dealWithExcpetion("Attempted initialization of named enum '" + DERIVED_ENUM_CLASS::getEnumName()
                                               + "' with invalid value '" + Utility::arithmeticToString(value) + "'");
        }
    }

    /*! \brief Construct from a std::string object.
     *
     * Construction from std::string means to look up the string inside the map and
     * to identify the corresponding ENUM_BASE_TYPE_VALUE. If the input string is not
     * found, an exception is thrown.
     */
    explicit NamedEnumBase(std::string const &value) :
        NamedEnumBase(NamedEnumBase::fromStringToBaseType(value))
    {}

    /*! \brief Construct from DUTIL::Variant object.
     *
     * This constructor delegates the construction to the NamedEnumBase(std::string const & value) constructor.
     */
    explicit NamedEnumBase(DUTIL::Variant const &variant) :
        NamedEnumBase(NamedEnumDetail::variantToString(variant))
    {}

    /*! \brief Conversion operator
     *
     * Define an implicit conversion operator which converts a NamedEnum value
     * in its underlying base type (by default, this base type is int or DUTIL::label_t).
     */
    operator ENUM_BASE_TYPE() const { return value_; }

    //! Return the current enum name.
    std::string toString() const
    {
        return NamedEnumBase::toString(value_);
    }

    //! Return the number of registered enum names.
    static label_t size()
    {
        auto map = getNameMap();
        return map.size();
    }

    //! Return a list containing all defined enum names.
    static StringList getAllowedNames()
    {
        return NamedEnumDetail::getListOfAllowedValues(getNameMap());
    }

protected:
    static ENUM_BASE_TYPE fromStringToBaseType(std::string input)
    {
        return NamedEnumDetail::fromStringToBaseType<ENUM_BASE_TYPE>(getNameMap(), getDefaultValue(), input);
    }

    static std::string toString(ENUM_BASE_TYPE aBaseTypeValue)
    {
        if (!isValidValue(aBaseTypeValue)) {
            NamedEnumDetail::dealWithExcpetion(
                "'" + Utility::arithmeticToString(aBaseTypeValue)
                + "' is not a registered base type value corresponding to a valid enum parameter.");
        }
        return getNameMap().at(aBaseTypeValue);
    }

    static void initNameMap(NamedEnumDetail::MapType<ENUM_BASE_TYPE> &map)
    {
        NamedEnumDetail::initNameMap(map, DERIVED_ENUM_CLASS::getEnumName(), DERIVED_ENUM_CLASS::getValueList());
    }

    static NamedEnumDetail::MapType<ENUM_BASE_TYPE> &getNameMap()
    {
        // static map at compile time
        static NamedEnumDetail::MapType<ENUM_BASE_TYPE> nameMap;
        if (nameMap.empty()) {
            initNameMap(nameMap);
        }
        return nameMap;
    }

    static ENUM_BASE_TYPE getDefaultValue() { return getNameMap().cbegin()->first; }

    static bool isValidValue(ENUM_BASE_TYPE key)
    {
        NamedEnumDetail::MapType<ENUM_BASE_TYPE> const &map = getNameMap();
        return (map.find(key) != map.cend());
    }

private:
    ENUM_BASE_TYPE value_;
};

} // namespace DUTIL

/*! \brief Macro for defining a named class that wrapps arund a user enum definition
 *
 * It is called "NAMED", since the actual class name is given as a macro parameter.
 *
 * EnumValues is a defined type name for the enum value field specified by __VA_ARGS__.
 * A "EnumValues" varible can only take values out of that field.
 * It means the same type as ENUM_TYPE does. __VA_ARGS__ is a set of alias names for possible
 * values of this type. The default type is DUTIL::label_t, see basictypes.h.
 *
 * This macro also defines several functions:
 *  - value(): casts an underlying named enum base type (e.g. int) to the actual enum type.
 *  - fromEnumValue: a friend function returning a ENUM_NAME object built from EnumValues parameter.
 *  - getEnumName: returns ENUM_NAME as a string.
 *  - getValueList: returns __VA_ARGS__ content as a single string.
 *  - toString: friend method to call protected NamedEnumBase::toString function.
 */
#define D_NAMED_WRAPPED_ENUM(ENUM_NAME, ENUM_TYPE, ...) \
    class ENUM_NAME : public DUTIL::NamedEnumBase<ENUM_NAME, ENUM_TYPE> \
    { \
    public: \
        using EnumValues = enum : ENUM_TYPE { __VA_ARGS__ }; \
        using DUTIL::NamedEnumBase<ENUM_NAME, ENUM_TYPE>::NamedEnumBase; \
        ENUM_NAME(EnumValues value) \
            : DUTIL::NamedEnumBase<ENUM_NAME, ENUM_TYPE>(value) \
        {} \
        ENUM_NAME() \
            : DUTIL::NamedEnumBase<ENUM_NAME, ENUM_TYPE>() \
        {} \
        EnumValues value() const \
        { \
            return static_cast<EnumValues>(ENUM_TYPE(*this)); \
        } \
        friend ENUM_NAME fromEnumValue(EnumValues const &value) \
        { \
            return ENUM_NAME(value); \
        } \
        friend std::string toString(EnumValues const &value) \
        { \
            return NamedEnumBase::toString(value); \
        } \
        static const std::string getEnumName() \
        { \
            return #ENUM_NAME; \
        } \
        static const std::string getValueList() \
        { \
            return #__VA_ARGS__; \
        } \
    };

//! Shortcut macro for defining a named enum with DUTIL::lable_t, that is int, its as base type
#define D_NAMED_ENUM(ENUM_NAME, ...) D_NAMED_WRAPPED_ENUM(ENUM_NAME, DUTIL::label_t, __VA_ARGS__)

#endif // DUTIL_NAMEDENUM_H
