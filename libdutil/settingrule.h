#ifndef DUTIL_SETTINGRULE_H
#define DUTIL_SETTINGRULE_H
#include "basictypes.h"
#include "variant.h"
#include <limits>

namespace DUTIL {
struct ConstructionData;

/*! \brief description of SettingRule
 *
 * Longer description of SettingRule.
 */

struct SettingRule
{
    D_NAMED_ENUM(Usage, OPTIONAL, MANDATORY_WITH_DEFAULT, MANDATORY_NO_DEFAULT)

    //! Define a set of public members characoterizing a SettingRule object.
    Usage usage;
    std::string key;
    std::string description;
    StringList listOfPossibleValues;
    Variant::Type type;
    Variant defaultValue;
    Variant minimalValue;
    Variant maximalValue;
    label_t minimalStringLength;

    /*! \brief Default construct a SettingRule object
     *
     * Type member is set to Type::STRING and usage is set to Usage::OPTIONAL.
     * All other members are empty or zero by default.
     */
    SettingRule();

    /*! \brief Create a SettingRule for a NamedEnum type.
     *
     * Usage:
     * Settingrule sr = SettingRule::forNamedEnum<NAMED_ENUM_TYPE>(usage, description);
     */
    template<typename NE, std::enable_if_t<std::is_enum_v<typename NE::EnumValues>, bool> = false>
    static SettingRule forNamedEnum(Usage u, std::string description)
    {
        SettingRule sr;
        sr.usage = u;
        sr.key = NE::getEnumName();
        sr.description = description;
        sr.listOfPossibleValues = NE::getAllowedNames();
        sr.type = Variant::Type::STRING;
        if (u != Usage::MANDATORY_NO_DEFAULT)
            sr.defaultValue = Variant(NE().value());
        return sr;
    }

    /*! \brief Create a SettingRule for a NamedParameter type.
     *
     * Usage:
     * Settingrule sr = SettingRule::forNamedParameter<NAMED_PARAMETER_TYPE>(usage, description);
     */
    template<typename NP>
    static SettingRule forNamedParameter(Usage u, std::string description)
    {
        SettingRule sr;
        Variant defaultValue = Variant(NP().value());
        sr.usage = u;
        sr.key = NP::getParameterName();
        sr.description = description;
        sr.type = defaultValue.getType();

        // min max values for named integer.
        if constexpr (BasicTypes::is_integer_v<typename NP::type>) {
            sr.minimalValue = Variant(std::numeric_limits<typename NP::type>::min());
            sr.maximalValue = Variant(std::numeric_limits<typename NP::type>::max());
        }

        // min max values for named floating point.
        if constexpr (std::is_floating_point_v<typename NP::type>) {
            sr.minimalValue = Variant(std::numeric_limits<typename NP::type>::lowest());
            sr.maximalValue = Variant(std::numeric_limits<typename NP::type>::max());
        }
        if (u != Usage::MANDATORY_NO_DEFAULT)
            sr.defaultValue = defaultValue;
        return sr;
    }
};

} // namespace DUTIL
#endif // DUTIL_SETTINGRULE_H
