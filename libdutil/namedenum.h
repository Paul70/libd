#ifndef DUTIL_NAMEDENUM_H
#define DUTIL_NAMEDENUM_H
#include "basictypes.h"
#include "utility.h"
#include <map>

// die map wird zur compile time komplett gebaut aus den VA_ARGS
// muss mir das noch anschauen, wie das mit der reference läuft und ob ich das so lassen kann

namespace DUTIL {
namespace NamedEnumDetail {

template<typename ENUM_BASE_TYPE>
using MapType = std::map<ENUM_BASE_TYPE, std::string>;

template<typename ENUM_BASE_TYPE>
void initNameMap(MapType<ENUM_BASE_TYPE> &map, std::string const &arg)
{
    StringList list = Utility::split(arg);

    // Default initial enum value, may be overwritten after string arg parsing.
    ENUM_BASE_TYPE initVal = 0;
    if (list.front().find('=') != std::string::npos) {
    }

    // fill map with all enum values
    for (auto &enumValue : list) {
        map.emplace(std::make_pair(initVal, enumValue));
        ++initVal;
    }
}

} // namespace NamedEnumDetail

/*! \brief description of NamedEnum
 *
 * NamedEnumBase follows the couriously recurring template pattern (CRTP).
 * DERIVED_ENUM_CLASS represents the "smart" enum class name, see also the macro D_NAMED_WRAPPED_ENUM.
 *
 */

template<typename DERIVED_ENUM_CLASS, typename ENUM_BASE_TYPE>
class NamedEnumBase
{
public:
    //! Check if enum base type is allowed and offers required, arithmetic operations.
    static_assert(!std::is_abstract_v<ENUM_BASE_TYPE>, "Enum Base Type has to be an arithmetic type.");

    NamedEnumBase() :
        value_(NamedEnumBase::getDefaultValue())
    {}

    NamedEnumBase(ENUM_BASE_TYPE value) :
        value_(value)
    {}

    /*! \brief Conversion operator
     *
     * Define an implicit conversion operator which converts a NamedEnum value
     * in its underlying base type (by default, this base type is int or DUTIL::label_t).
     */
    operator ENUM_BASE_TYPE() const
    {
        return value_;
    }

protected:
    static void initNameMap(NamedEnumDetail::MapType<ENUM_BASE_TYPE> &map)
    {
        NamedEnumDetail::initNameMap(map, DERIVED_ENUM_CLASS::getValueList());
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

    static ENUM_BASE_TYPE getDefaultValue()
    {
        return getNameMap().cbegin()->first;
    }

private:
    ENUM_BASE_TYPE value_;
};

} // namespace DUTIL

/*! \brief Macro for defining a named class that wrapps arund a user enum definition
 *
 * It is called named, since the actual class name given as a macro parameter.
 *
 * EnumValues is a defined type name for the enum value field specified by __VA_ARGS__.
 * A "EnumValues" varible can only take values out of that field.
 * It means the same type as ENUM_TYPE does. __VA_ARGS__ is a set of alias names for possible
 * values of this type. The default type is DUTIL::label_t, see basictypes header.
 *
 * NamedEnumBase is the interface class for ENUM_NAME class.
 */
#define D_NAMED_WRAPPED_ENUM(ENUM_NAME, ENUM_TYPE, ...)                 \
class ENUM_NAME : public DUTIL::NamedEnumBase<ENUM_NAME, ENUM_TYPE>     \
{                                                                       \
public:                                                                 \
    using EnumValues = enum : ENUM_TYPE {__VA_ARGS__};                  \
    ENUM_NAME(EnumValues value) :                                       \
        DUTIL::NamedEnumBase<ENUM_NAME, ENUM_TYPE>(value)               \
    {}                                                                  \
    ENUM_NAME()                                                         \
        : DUTIL::NamedEnumBase<ENUM_NAME, ENUM_TYPE>()                  \
    {}                                                                  \
    static const std::string getEnumName()                              \
    {                                                                   \
        return #ENUM_NAME;                                              \
    }                                                                   \
    static const std::string getValueList()                             \
    {                                                                   \
        return #__VA_ARGS__;                                            \
    }                                                                   \
};

//! Shortcut macro for defining a named enum with DUTIL::lable_t, that is int, its as base type
#define D_NAMED_ENUM(ENUM_NAME, ...) D_NAMED_WRAPPED_ENUM(ENUM_NAME, DUTIL::label_t, __VA_ARGS__)
#endif // DUTIL_NAMEDENUM_H
