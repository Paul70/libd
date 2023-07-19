#ifndef DUTIL_CONSTRUCTIONVALIDATOR_H
#define DUTIL_CONSTRUCTIONVALIDATOR_H
#include "settingrule.h"
#include <functional>
#include <string>

namespace DUTIL {
struct ConstructionData;
/*! \brief description of ConstructionValidator
 *
 * Longer description of ConstructionValidator.
 */

class ConstructionValidator
{
public:
    //! Define a fucntion callback type for custom check functions examining ConstructionData objects.
    using CheckFunction = std::function<std::string(ConstructionValidator const &, ConstructionData const &)>;

    //! A trivial ceck function for (empty) default construcion.
    static std::string trivialCheck(ConstructionValidator const &, ConstructionData const &)
    {
        return "";
    }

    //! Default check function for validating ConstructionData.
    static std::string recursiveCheck(ConstructionValidator const &cv, ConstructionData const &cd);

    //! Default Constructor.
    ConstructionValidator();

    //! Construct with construction data
    explicit ConstructionValidator(std::vector<SettingRule> settingRules,
                                   ConstructionValidator baseCV = {},
                                   CheckFunction cf = recursiveCheck);

    /*! \brief Use the check function defined at construction to check the given ConstructionData object.
     *
     * Any errors are returned in string form, an empty string implies that no errors were found.
     * This function is always available since a ConstructionValidator object is meant to be static,
     * see ConcreteWare::getConstructionValidator function.
     * Main purpose ot the this function is to call the ckeck_ functor which is responsible for calling
     *  1. check_ functors of base class ConstructionValidators,
     *  2. the default 'recursiveCheck' function and
     *  3. optional custom check-fucntions defined in ConcreteWare objects.
     */
    std::string check(ConstructionData const &cd) const;

    //! Return SettingRule object specified by key. If no rule is found, an exception is thrown.
    SettingRule getSettingRule(std::string const &key) const;

    //! Return all setting rule keys. If there are no setting rules, the list will be empty.
    StringList getListOfSettingRuleKeys() const;

    // functions for extraction

    template<typename NE, std::enable_if_t<std::is_enum_v<typename NE::EnumValues>, bool> = false>
    NE validateNamedEnum(ConstructionData const &cd) const
    {
        return NE(validateSettingRuleKeyAndReturnValue(cd, NE::getEnumName()));
    }
    template<typename NP>
    NP validateNamedParameter(ConstructionData const &cd) const
    {
        return NP(validateSettingRuleKeyAndReturnValue(cd, NP::getParameterName()));
    }

    // check functions
    template<typename NE, std::enable_if_t<std::is_enum_v<typename NE::EnumValues>, bool> = false>
    std::string checkNamedEnum(ConstructionData const &cd)
    {
        return checkSettingRuleKeyAndReturnError(cd, NE::getEnumName());
    }

private:
    Variant checkSettingRuleKeyAndReturnValue(Variant const value, std::string const &key, std::string &error) const;
    std::string checkSettingRuleKeyAndReturnErrors(ConstructionData const &cd, std::string const &key) const;

    Variant validateSettingRuleKeyAndReturnValue(ConstructionData const &cd, std::string const key) const;

    std::map<std::string, SettingRule> settingRules_;
    CheckFunction check_;
};

} // namespace DUTIL
#endif // DUTIL_CONSTRUCTIONVALIDATOR_H
