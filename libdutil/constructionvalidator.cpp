#include "constructionvalidator.h"
#include "constructiondata.h"
#include "exception.h"
#include <iostream>

namespace DUTIL {

namespace {
bool checkASmallerThanB(Variant::Type type, Variant const &A, Variant const &B)
{
    if (A.isMonostate() || B.isMonostate()) {
        return false;
    }

    if (!Variant::isNumeric(type))
        return false;

    using T = Variant::Type;
    if (type == T::INT64) {
        if (A.getAs<std::int64_t>() > B.getAs<std::int64_t>())
            return false;
    }
    if (type == T::LABEL) {
        if (A.getAs<label_t>() > B.getAs<label_t>())
            return false;
    } else if (type == T::DOUBLE) {
        if (A.getAs<double>() > B.getAs<double>())
            return false;
    } else {
        if (A.getAs<std::uint64_t>() > B.getAs<std::uint64_t>())
            return false;
    }
    return true;
}

bool checkPresenceInListOfAllowedValues(StringList const &haystack, std::string const &needle)
{
    if (haystack.empty())
        return false;
    if (std::find(haystack.cbegin(), haystack.cend(), needle) != haystack.cend()) {
        return true;
    }
    return false;
}

SettingRule checkSettingRule(SettingRule sr)
{
    // This function checks if the SettingRule itself is valid, idependently from
    // ConstructionData

    D_ASSERT(!sr.key.empty());
    D_ASSERT(sr.defaultValue.isValid() || sr.usage != SettingRule::Usage::MANDATORY_WITH_DEFAULT);
    D_ASSERT(sr.defaultValue.isMonostate() || sr.usage != SettingRule::Usage::MANDATORY_NO_DEFAULT);
    D_ASSERT(sr.listOfPossibleValues.empty() || sr.type == Variant::Type::STRING);

    if (sr.minimalValue.isValid()) {
        D_ASSERT(Variant::isNumeric(sr.type));
        Variant original = sr.minimalValue;
        sr.minimalValue.convertTo(sr.type);
        Variant converted = sr.minimalValue;
        D_ASSERT(original == converted.convertTo(original.getType()));
    }
    if (sr.maximalValue.isValid()) {
        D_ASSERT(Variant::isNumeric(sr.type));
        Variant original = sr.maximalValue;
        sr.maximalValue.convertTo(sr.type);
        Variant converted = sr.maximalValue;
        D_ASSERT(original == converted.convertTo(original.getType()));
    }
    if (sr.minimalValue.isValid() && sr.maximalValue.isValid()) {
        D_ASSERT_MSG(checkASmallerThanB(sr.type, sr.minimalValue, sr.maximalValue),
                     "Invalid setting rule after ckecking min and max value limits.");
    }
    if (!sr.listOfPossibleValues.empty() && sr.defaultValue.isValid()) {
        D_ASSERT_MSG(checkPresenceInListOfAllowedValues(sr.listOfPossibleValues, sr.defaultValue.toString()),
                     "Invalid setting rule after ckecking of list of possible values.");
    }
    return sr;
}

WarelistRule checkWarelistRule(WarelistRule wr)
{
    // check that callbackCV contains a reference to the static 'ConstructionValidator' function
    std::string cbName{wr.callbackCV.target_type().name()};
    D_ASSERT(cbName.find("ConstructionValidator") != std::string::npos);

    // 0 length is not meaningful
    D_ASSERT(wr.length != 0);

    return wr;
}
} // namespace

std::string ConstructionValidator::recursiveCheck(ConstructionValidator const &cv, ConstructionData const &cd)
{
    // check all available setting rules
    std::string errors;
    for (auto const &iter : cv.settingRules_) {
        errors = cv.checkSettingRuleKeyAndReturnErrors(cd, iter.first);
        if (!errors.empty()) {
            return errors;
        }
    }

    // check warelist rules for each subobject in cd
    for (auto &iter : cv.warelistRules_) {
        WarelistRule const &wlr = iter.second;
        // hier muss dann noch die Abfrage for shared wares rein
        if (wlr.length == 1) {
            // single subobject.
            errors = cv.checkSubObjectAndReturnErrors(cd, iter.first);
        } else {
            // list of subobjects all of the same type, therefore there is only one warelist rule.
            errors = cv.checkSubObjectListAndReturnErrors(cd, iter.first);
        }
        if (!errors.empty()) {
            return errors;
        }
    }

    if (!errors.empty())
        D_THROW("At this point, construction validator error string has to be empty.");
    return errors;
}

std::string ConstructionValidator::typeSettingCheck(ConstructionData const &cd)
{
    if (cd.s.hasKey("Type")) {
        return "";
    }
    return "Setting a type parameter in consturciton data is mandatory in this case, see "
           "'ConstructionData::setConcreteClassParameter()'.";
}

bool ConstructionValidator::proxyCheck(ConstructionData const &cd)
{
    if (cd.isProxy())
        return true;
    else
        return false;
}

ConstructionValidator::ConstructionValidator() :
    settingRules_(),
    check_(trivialCheck)
{}

ConstructionValidator::ConstructionValidator(std::vector<SettingRule> settingRules,
                                             std::vector<WarelistRule> warelistRules,
                                             ConstructionValidator baseCV,
                                             CheckFunction checkF) :
    settingRules_(baseCV.settingRules_),
    warelistRules_(baseCV.warelistRules_),
    check_(baseCV.check_)
{
    // put all setting rules into the map.
    for (auto const &sr : settingRules) {
        settingRules_.emplace(sr.key, checkSettingRule(sr));
    }

    // put all the given warelist rules into the map
    for (auto const &wr : warelistRules) {
        warelistRules_.emplace(wr.key, checkWarelistRule(wr));
    }

    if (checkF) {
        // assign a lambda to the check funtor
        // lambda first calls the check-function (not directly the functor) of the base construction validator.
        // In case of no errors, the custom check functor of this validator gets called.
        check_ = [=](ConstructionValidator const &cvThis, ConstructionData const &cdThis) {
            auto errors = baseCV.check_(cvThis, cdThis);
            if (!errors.empty())
                return errors;
            return checkF(cvThis, cdThis);
        };
    }
}

std::string ConstructionValidator::check(ConstructionData const &cd) const
{
    // Crucial step:
    // either this callback tiggers the default check routine usding 'recursiveCheck()' or omitts
    // the validation porcess in case a CD object is only a proxy and not used to construct a real object.
    if (cd.isProxy())
        return "";
    else
        return check_(*this, cd);
}

bool ConstructionValidator::hasSettingRule(std::string const &key) const
{
    if (settingRules_.find(key) == settingRules_.end())
        return false;
    return true;
}

SettingRule ConstructionValidator::getSettingRule(std::string const &key) const
{
    auto rule = settingRules_.find(key);
    if (rule == settingRules_.end()) {
        D_THROW("No SettingRule found for given key '" + key + "'.");
    }
    return rule->second;
}

StringList ConstructionValidator::getListOfSettingRuleKeys() const
{
    StringList list;
    for (auto const &it : settingRules_) {
        list.push_back(it.first);
    }
    return list;
}

Variant ConstructionValidator::checkSettingRuleKeyAndReturnValue(Variant const value,
                                                                 std::string const &key,
                                                                 std::string &error) const
{
    auto sr = getSettingRule(key);

    // clang-format off
    if (sr.usage == SettingRule::Usage::MANDATORY_NO_DEFAULT && value.isMonostate()) {
        error = "SettingRule for key '"
                + key
                + "' requires a value offered by consruction data and does not support setting a default value.";
        return Variant();
    }
    if (sr.usage == SettingRule::Usage::MANDATORY_NO_DEFAULT && sr.defaultValue.isValid()) {
        error = "Setting rule for key '"
                + key + "' and value: " + value.toString()
                + "is mandatory and cannot rely on a default value.";
        return Variant();
    }
    if (sr.usage == SettingRule::Usage::MANDATORY_WITH_DEFAULT && value.isMonostate()) {
        // checking the default value's validility is done before
        // inside the checkSettingRule() function.
        return sr.defaultValue;
    }
    if (sr.usage == SettingRule::Usage::OPTIONAL && value.isMonostate()) {
        // optional value is not set in ConstructionData.
        return Variant();
    }

    // ckeck Variant type
    if (value.getType() != sr.type && value.isValid()) {
        error = "Setting rule for key '" + key + "' and value: " + value.toString()
                + "defines a different type for the parameter";
        return Variant();
    }

    // check list of possible values
    if (!sr.listOfPossibleValues.empty() && value.isValid()
        && !checkPresenceInListOfAllowedValues(sr.listOfPossibleValues, value.toString())) {
        error = "Setting rule for key '"
                + key + "' allows the following values: ";
        for (auto const &it : sr.listOfPossibleValues) {
            error += it + ",";
        }
        error += ". '" + value.toString() + "' is not in that list.";
        return Variant();
    }

    // check min/max limits
    if (sr.minimalValue.isValid() && !checkASmallerThanB(sr.type, sr.minimalValue, value)) {
        error = "Setting for key '"
                + key + "' and value: " + value.toString()
                + "is smaller than the allowed min value.";
        return Variant();
    }
    if (sr.maximalValue.isValid() && !checkASmallerThanB(sr.type, value, sr.maximalValue)) {
        error = "Setting for key '"
                + key + "' and value: " + value.toString()
                + "is bigger than the allowed max value.";
        return Variant();
    }

    // check max string length
    if (sr.type == Variant::Type::STRING && label_t(value.toString().size()) < sr.minimalStringLength) {
        error = "Setting for key '"
                + key + "' and value: " + value.toString()
                + "requires a min string length of "
                + Utility::toString(sr.minimalStringLength) + ".";
        return Variant();
    }
    // clang-format on
    return value;
}

/*! \brief Anonymous namespace containing helper functions.
 *
 * - 'getFirstSubobjectAndNumberOfKeyOccurences'
 *   Returns a std::pair where the first entry is an iterator pointing to the first subobject referring to the given key
 *   and the second entry says how many subobject cds are available for that key.
 * - 'checkSubobjectRecursivelyAndReturnErrors'
 *   checks a single subobject cd recursevely, i.e. also checks nested subobject inside this subobject and eventually return
 *   an error string. If there are no errors that string is empty.
 * - 'checkSubobjectListRecursevilyAndReturnErrors'
 *   Does the same as 'checkSubobjectRecursivelyAndReturnErrors' however for more than one subobject cd. All subobject cds
 *   refer to the same type to construct.
 */
namespace {

// If no subobject for the given key is found, this fucntion retrun a past the end iterator.
template<typename ListType>
auto getFirstSubobjectAndNumberOfKeyOccurences(ListType const &subobjects, std::string const &key)
{
    size_t count = 0;
    auto prefix = key + ConstructionData::seperator;
    auto index = subobjects.find(prefix + Utility::toString(count));
    if (index == subobjects.cend()) {
        return std::make_pair(index, count);
    }

    auto result = std::make_pair(index, ++count);
    while (subobjects.find(prefix + Utility::toString(count)) != subobjects.cend()) {
        ++count;
    }
    result.second = count;
    return result;
}

template<typename ListType>
std::string checkSubobjectRecursivelyAndReturnErrors(
    ListType const &subojects,          /*container holding all subobject CD at this level*/
    std::string const &key,             /*key to identify the desired subobject inside the subobject container*/
    ConstructionValidator const &subCV, /*ConstructionValidator reference to be used to validate the subobject cd*/
    std::map<std::string, WarelistRule> const &
        warelistRules) /*a map containing all warelist rules, use key to identify the warelist rule for the subobject cd to be validated*/
{
    WarelistRule wlr = warelistRules.at(key);
    auto subData = getFirstSubobjectAndNumberOfKeyOccurences(subojects, key);

    if (subData.second == 0) {
        if (wlr.usage == WarelistRule::Usage::MANDATORY) {
            // clang-format off
            return "Checking single subobject for key " +
                    key +
                    ": No subobject found.";
            // clang-format on
        } else {
            return "";
        }
    }

    // the actual recusive call which triggers the whole validation process for sub-subobjects
    std::string error = subCV.check(subData.first->second);
    if (!error.empty()) {
        return "In subobject data for '" + key + "':\n  " + error;
    }
    error = ConstructionValidator::typeSettingCheck(subData.first->second);
    if (!error.empty()) {
        return "In subobject data for '" + key + "':\n  " + error;
    }
    return "";
}

template<typename ListType>
std::string checkSubobjectListRecursevilyAndReturnErrors(
    ListType const &subobjects,         /*container holding all subobject CD at this level*/
    std::string const &key,             /*key to identify the desired subobject inside the subobject container*/
    ConstructionValidator const &subCV, /*ConstructionValidator reference to be used to validate the subobject cd*/
    WarelistRule const &
        warelistrule) /*a map containing all warelist rules, use key to identify the warelist rule for the subobject cd to be validated*/
{
    auto subData = getFirstSubobjectAndNumberOfKeyOccurences(subobjects, key);

    // Check if there are enough subobject CDs available if.
    // The Usage::OPTIONAL case is only valid, if there are no subobjects defined at all for the given key.
    // That is, if there are subobject CDs defined, their number has to match the length given in the rule.
    if (subData.second == 1) {
        return "There are not enough subobject CDs for key" + key;
    } else if (subData.second == 0) {
        if (warelistrule.usage == WarelistRule::Usage::MANDATORY) {
            // clang-format off
            return "Checking list of subobjects for key " +
                    key +
                    ": No subobjects found at all.";
            // clang-format on
        } else {
            return "";
        }
    } else if (warelistrule.length != WarelistRule::lengthNotDefined
               && static_cast<label_t>(subData.second) != warelistrule.length) {
        // clang-format off
        return "Checking list of subobjects for key " +
                key +
                ": The required number of subobjects (" +
                Utility::toString(warelistrule.length) +
                ") does not match the actual number:" +
                Utility::toString(subData.second) +
                " inside the given CD.";
        // clang-format on
    }

    // Recusively check all subobject CDs.
    for (size_t i = 0; i < subData.second; ++i) {
        std::string error = subCV.check(subData.first->second);
        if (!error.empty())
            return "In subobject data for '" + key + "':\n  " + error;
        error = ConstructionValidator::typeSettingCheck(subData.first->second);
        if (!error.empty())
            return "In subobject data for '" + key + "':\n  " + error;
        ++(subData.first);
    }
    return "";
}

} // namespace

std::string ConstructionValidator::checkSettingRuleKeyAndReturnErrors(ConstructionData const &cd, std::string const &key) const
{
    std::string error;
    auto keyList = cd.s.keys();
    for (auto const &cdKey : keyList) {
        if (cd.s.value(cdKey).isValid() && !hasSettingRule(cdKey)) {
            return error = "ConstructionData key '" + cdKey + "' does not match any SettingRule key.";
        }
    }
    checkSettingRuleKeyAndReturnValue(cd.s.value(key), key, error);
    return error;
}

std::string ConstructionValidator::checkSubObjectAndReturnErrors(ConstructionData const &cd, std::string const &key) const
{
    // cd is the superior ConstructionData object containing all subobject cds.
    // key identifies the warelistrule for the subobjects type in the ConstructionValidator map.

    // check if a warelistrule really exist for given key
    if (warelistRules_.find(key) == warelistRules_.cend()) {
        return "There is no registered warelistrule for key '" + key + "'. It is not possible to validate the subobject.";
    }

    // check if warelistrule refers to a single object
    auto wlr = warelistRules_.at(key);
    if (wlr.length != WarelistRule::lengthSingleton) {
        return "Warelistrule for key '" + key + "' refers to a subobject list. Call 'checkSubobjectListAndReturnErrors' instead.";
    }

    // get the subobject's CV
    auto &subobjectCV = wlr.callbackCV();
    return checkSubobjectRecursivelyAndReturnErrors(cd.subObjectData, key, subobjectCV, warelistRules_);
}

std::string ConstructionValidator::checkSubObjectListAndReturnErrors(ConstructionData const &cd, std::string const &key) const
{
    // cd is the superior ConstructionData object containing all subobject cds.
    // key identifies the warelistrule for the subobjects type in the ConstructionValidator map.

    // Check if a warelistrule really exists for the given key
    if (warelistRules_.find(key) == warelistRules_.cend()) {
        return "There is no registered warelistrule for key '" + key + "'. It is not possible to validate the subobjects.";
    }

    auto wlr = warelistRules_.at(key);

    // Check if there are more then one subobjects building objects of the same type
    if (wlr.length == WarelistRule::lengthSingleton) {
        return "Warelistrule for key '" + key + "' refers to a single subobject. Call 'checkSubobjectAndReturnErrors' instead.";
    }

    // get the subobject's CV and call the recursive template check function.
    auto &subobjectCV = wlr.callbackCV();
    return checkSubobjectListRecursevilyAndReturnErrors(cd.subObjectData, key, subobjectCV, wlr);
}

Variant ConstructionValidator::validateSettingRuleKeyAndReturnValue(ConstructionData const &cd, std::string const key) const
{
    std::string error;
    Variant value = checkSettingRuleKeyAndReturnValue(cd.s.value(key), key, error);
    if (!error.empty())
        D_THROW(error);
    return value;
}

ConstructionData const &ConstructionValidator::validateAndReturnSubObjectCD(ConstructionData const &cd,
                                                                            std::string const key) const
{
    static const ConstructionData subObjectCD = ConstructionData(ConstructionData::Usage::PROXY);
    std::string error = checkSubObjectAndReturnErrors(cd, key);
    if (!error.empty()) {
        D_THROW(error);
    }
    // if no error was detected, extract nested ConstructionData structure from map.
    auto iter = cd.subObjectData.find(key + ConstructionData::seperator + "0");
    if (iter != cd.subObjectData.cend()) {
        return iter->second;
    } else {
        // if nothing is found return default construction data
        return subObjectCD;
    }
}

std::vector<ConstructionData const *> ConstructionValidator::validateAndReturnSubobjectCDs(ConstructionData const &cd,
                                                                                           std::string const &key) const
{
    static const ConstructionData proxyCD = ConstructionData(ConstructionData::Usage::PROXY);
    std::string error = checkSubObjectListAndReturnErrors(cd, key);
    if (!error.empty()) {
        D_THROW(error);
    }

    // no error detected, return a vector with pointers to the subobject cds.
    auto count = 0;
    auto iter = cd.subObjectData.find(key + ConstructionData::seperator + Utility::toString(count));
    if (iter == cd.subObjectData.cend()) {
        return std::vector<ConstructionData const *>{&proxyCD};
    } else {
        std::vector<ConstructionData const *> result(cd.subObjectData.size());
        do {
            result[count] = &iter->second;
            iter = cd.subObjectData.find(key + ConstructionData::seperator + Utility::toString(++count));
        } while (iter != cd.subObjectData.cend());
        return result;
    }
}
} // namespace DUTIL
