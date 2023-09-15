#ifndef DUTIL_CONSTRUCTIONVALIDATOR_H
#define DUTIL_CONSTRUCTIONVALIDATOR_H
#include "settingrule.h"
#include "warelistrule.h"
#include <functional>
#include <memory>
#include <string>
#include <type_traits>

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

    //! Define a check function which validates if the given ConstructionData referece refers to a real or a proxy object.
    static bool proxyCheck(ConstructionData const &cd);

    //! Default Constructor.
    ConstructionValidator();

    //! Construct with construction data
    explicit ConstructionValidator(std::vector<SettingRule> settingRules,
                                   std::vector<WarelistRule> warelistRules = {},
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

    //! Check if a setting rule associated with the given key exists.
    bool hasSettingRule(std::string const &key) const;

    //! Return SettingRule object specified by key. If no rule is found, an exception is thrown.
    SettingRule getSettingRule(std::string const &key) const;

    //! Return all setting rule keys. If there are no setting rules, the list will be empty.
    StringList getListOfSettingRuleKeys() const;

    /*! \brief Extraction fucntions.
     *
     * These functions perform a validation before they retrun values or nested ConstructionData
     * objects from the overall ConstructionData object.
     */
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

    template<typename NR>
    NR validateNamedReference()
    {
        return NR();
    }

    /*! \brief Dynamically create one or more subobjects form nested construction data.
     *
     * Construction data is validated before objects are created.
     * See the helper function in the private section to understand how objects are built.
     *
     * IMPORTANT:
     * This function may return a nullptr in case in case the input ConstructionData does not
     * contain a nested ConstructionData object to build an optional subobject.
     * Then, it is the job of the user of this fucntion to deal with this nullptr correctly.
     *
     */
    template<typename NR>
    auto buildSubObject(ConstructionData const &cd) const -> std::unique_ptr<typename NR::RT>
    {
        // Note, cd refers to the overall and not the the subobject construction data, here.
        auto subCD = validateAndReturnSubObjectCD(cd, NR::getReferenceName());
        if (proxyCheck(subCD)) {
            return nullptr;
        }
        return makeObjectHelper<typename NR::RT>(subCD);
    }

    template<typename NR>
    auto buildSubobjectList(ConstructionData const &cd) const
    {
        // vector of subobject cd
    }

    // check functions
    template<typename NE, std::enable_if_t<std::is_enum_v<typename NE::EnumValues>, bool> = false>
    std::string checkNamedEnum(ConstructionData const &cd) const
    {
        return checkSettingRuleKeyAndReturnError(cd, NE::getEnumName());
    }

    template<typename NR>
    std::string checkSubObject(ConstructionData const &cd) const
    {
        return checkSubObjectAndReturnErrors(cd, NR::getReferenceName());
    }

private:
    Variant checkSettingRuleKeyAndReturnValue(Variant const value, std::string const &key, std::string &error) const;

    /*! \brief Check functions.
     *
     * This set of functions are calles from their validate... counterpart functions and
     * take a registered setting or warelist rule to test if the construction data parameter or construction data subobject
     * fullfills that rule.
     */
    std::string checkSettingRuleKeyAndReturnErrors(ConstructionData const &cd, std::string const &key) const;
    std::string checkSubObjectAndReturnErrors(ConstructionData const &cd, std::string const &key) const;

    /*! \brief Validation functions.
     *
     * This set of functions checks
     * - if s setting rule or warelist rule is available for the construction data parameter associated with the gieven key,
     * - calls the corresponding ckeck... function to test if the parameter fulfills the setting or warelist rule and
     * - crates a DUTIL::Variant containing the parameter value extracted from construction data structure or a whole
     *   ConstructionData struct itself to build subobjects.
     */
    Variant validateSettingRuleKeyAndReturnValue(ConstructionData const &cd, std::string const key) const;
    ConstructionData const &validateAndReturnSubObjectCD(ConstructionData const &cd, std::string const key) const;
    std::vector<ConstructionData const *> validateAndReturnSubobjectCDs(ConstructionData const &cd, std::string const &key) const;

    /*! \brief Helper functions.
     *
     * These functions help to call factories to create one or more objects during runtime.
     */
    template<typename T, std::enable_if_t<std::is_constructible_v<T, ConstructionData>, bool> = false>
    static std::unique_ptr<T> makeObjectHelper(ConstructionData const &cd)
    {
        return std::make_unique<T>(cd);
    };

    //! Members
    std::map<std::string, SettingRule> settingRules_;
    std::map<std::string, WarelistRule> warelistRules_;
    CheckFunction check_;
};

} // namespace DUTIL
#endif // DUTIL_CONSTRUCTIONVALIDATOR_H
