#include "constructionvalidator.h"
#include "constructiondata.h"
#include "exception.h"
#include "factory.h"
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
    D_ASSERT(!wr.key.empty());
    D_ASSERT(!wr.type.empty());

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
        if (wlr.asReference) {
            if (wlr.length == 1) {
                // single shared ware.
                errors = cv.checkSharedWareAndReturnErrors(cd, iter.first, wlr.type);
            } else {
                // list of shared wares all of the same type, therefore there is only one warelist rule.
                errors = cv.checkSharedWareListAndReturnErrors(cd, iter.first, wlr.type);
            }
        } else {
            if (wlr.length == 1) {
                // single subobject.
                errors = cv.checkSubObjectAndReturnErrors(cd, iter.first);
            } else {
                // list of subobjects all of the same type, therefore there is only one warelist rule.
                errors = cv.checkSubObjectListAndReturnErrors(cd, iter.first);
            }
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
    // Put all setting rules and warelist rules into the maps.
    // Base class construction validator rules get overwritten by new rules referring to the same key.
    for (auto const &sr : settingRules) {
        settingRules_[sr.key] = checkSettingRule(sr);
    }
    for (auto const &wr : warelistRules) {
        warelistRules_[wr.key] = checkWarelistRule(wr);
    }

    // Set the check function.
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

bool ConstructionValidator::hasWarelistRule(std::string const &key) const
{
    if (warelistRules_.find(key) == warelistRules_.end())
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
        // Optional value is not set in ConstructionData. In that case, if construction data is not marked explicitly as proxy,
        // we will return a default value if available or a monostate variant instead.
        return sr.defaultValue;
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
                + key + "' and value: '" + value.toString()
                + "' is smaller than the allowed min value.";
        return Variant();
    }
    if (sr.maximalValue.isValid() && !checkASmallerThanB(sr.type, value, sr.maximalValue)) {
        error = "Setting for key '"
                + key + "' and value: '" + value.toString()
                + "' is bigger than the allowed max value.";
        return Variant();
    }

    // check max string length
    if (sr.type == Variant::Type::STRING && label_t(value.toString().size()) < sr.minimalStringLength) {
        error = "Setting for key '"
                + key + "' and value: '" + value.toString()
                + "' requires a min string length of "
                + Utility::toString(sr.minimalStringLength) + ".";
        return Variant();
    }
    // clang-format on
    return value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace {
/*! \brief Easily access the firs subobject and get the number of key occurences.
 *
 *   Returns a std::pair where the first entry is an iterator pointing to the first subobject referring to the given key
 *   and the second entry says how many subobject cds are available for that key.
 *   If no subobject for the given key is found, this fucntion retrun a past the end iterator.
 */
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

/*! \brief Checks a shared ware pointer.
 *
 * Function basically checks if the names reference type and its poiter fit together.
 */
template<typename ListType>
std::string checkWareObjectAndReturnErrors(ListType const &wares,
                                           std::string const &key,
                                           std::string const &referredTypeName,
                                           WarelistRule const &warelistrule)
{
    if (referredTypeName != warelistrule.type) {
        return "ware rule for '" + warelistrule.key + "': type mismatch: should be '" + warelistrule.type
               + "' according to rule, but was passed a named reference to an object of type '" + referredTypeName + "'";
    }

    auto wareData = getFirstSubobjectAndNumberOfKeyOccurences(wares, key);
    if (wareData.second == 0) {
        if (warelistrule.usage == WarelistRule::Usage::MANDATORY) {
            // clang-format off
            return "Checking single shared ware for key " +
                    key +
                    ": No shared ware object found in construction data.";
            // clang-format on
        } else {
            return "";
        }
    }

    if (!wareData.first->second) {
        return "ware rule for '" + warelistrule.key + "': was passed empty ware pointer with ware key '" + key
               + "' in the construction data";
    }

    auto concreteClassName = (wareData.first->second)->getConcreteClassName();
    if (Factory::isRegisteredInterface(referredTypeName)) {
        if (!Factory::isRegisteredWithInterface(referredTypeName, concreteClassName)) {
            return "ware rule for '" + warelistrule.key
                   + "': type mismatch: object should be an instance of a class derived from '" + warelistrule.type
                   + "' according to rule, but was passed an object of incorrect type '" + concreteClassName
                   + "' in the construction data";
        }
    } else {
        if (referredTypeName != concreteClassName) {
            return "ware rule for '" + warelistrule.key + "': type mismatch: should be '" + warelistrule.type
                   + "' according to rule, but was passed an object of type '" + concreteClassName + "' in the construction data";
        }
    }
    return "";
}

template<typename ListType>
std::string checkWareListRecursivelyAndReturnErrors(ListType const &wares,
                                                    std::string const &key,
                                                    std::string const &referredTypeName,
                                                    WarelistRule const &warelistrule)
{
    if (referredTypeName != warelistrule.type) {
        return "ware rule for '" + warelistrule.key + "': type mismatch: should be '" + warelistrule.type
               + "' according to rule, but was passed a named reference to an object of type '" + referredTypeName + "'";
    }

    auto wareData = getFirstSubobjectAndNumberOfKeyOccurences(wares, key);
    if (wareData.second == 0) {
        if (warelistrule.usage == WarelistRule::Usage::MANDATORY) {
            // clang-format off
            return "Checking shared ware list for key " +
                    key +
                    ": No shared ware object found in construction data.";
            // clang-format on
        } else {
            return "";
        }
    }
    if (warelistrule.length != WarelistRule::lengthNotDefined && static_cast<label_t>(wareData.second) != warelistrule.length) {
        // clang-format off
        return "Checking list of shared wares for key " +
                key +
                ": The required number of shared wares (" +
                Utility::toString(warelistrule.length) +
                ") does not match the actual number, length mismatch:" +
                Utility::toString(wareData.second) +
                " inside the given CD shared ware map.";
        // clang-format on
    }

    // Recusively check all subobject CDs.
    for (size_t i = 0; i < wareData.second; ++i) {
        if (!wareData.first->second) {
            return "ware rule for '" + warelistrule.key + "': was passed empty ware pointer with ware key '" + key
                   + "' in the construction data";
        }
        auto concreteClassName = (wareData.first->second)->getConcreteClassName();
        if (Factory::isRegisteredInterface(referredTypeName)) {
            if (!Factory::isRegisteredWithInterface(referredTypeName, concreteClassName)) {
                return "ware list rule for '" + warelistrule.key + "': type mismatch: object '" + key
                       + ConstructionData::seperator + std::to_string(i) + "' should be an instance of a class derived from '"
                       + warelistrule.type + "' according to rule, but was passed an object of incorrect type '"
                       + concreteClassName + "' in the construction data";
            }
        } else {
            if (referredTypeName != concreteClassName) {
                return "ware list rule for '" + warelistrule.key + "': type mismatch: object '" + key
                       + ConstructionData::seperator + std::to_string(i) + "' should be '" + warelistrule.type
                       + "' according to rule, but was passed an object of type '" + concreteClassName
                       + "' in the construction data";
            }
        }
    }
    return "";
}

/*! \brief Check a single subobject.
 *
 *   checks a single subobject cd recursevely, i.e. also checks nested subobject inside this subobject and eventually return
 *   an error string. If there are no errors that string is empty.
 */
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
            return "Subobject data for '"
                   + key + "' was not passed in construction data";
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
    return "";
}

/*! \brief Check a list of subobjects.
 *
 *   Does the same as 'checkSubobjectRecursivelyAndReturnErrors' however for more than one subobject cd. All subobject cds
 *   refer to the same type to construct.
 */
template<typename ListType>
std::string checkSubobjectListRecursevilyAndReturnErrors(
    ListType const &subobjects,         /*container holding all subobject CD at this level*/
    std::string const &key,             /*key to identify the desired subobject inside the subobject container*/
    ConstructionValidator const &subCV, /*ConstructionValidator reference to be used to validate the subobject cd*/
    WarelistRule const &
        warelistrule) /*a map containing all warelist rules, use key to identify the warelist rule for the subobject cd to be validated*/
{
    auto subData = getFirstSubobjectAndNumberOfKeyOccurences(subobjects, key);

    // clang-format off
    // Check if there are enough subobject CDs available if.
    // The Usage::OPTIONAL case is only valid, if there are no subobjects defined at all for the given key.
    // That is, if there are subobject CDs defined, their number has to match the length given in the rule.
    if (subData.second == 1) {
        return "There are not enough subobject CDs for key" + key;
    } else if (subData.second == 0) {
        if (warelistrule.usage == WarelistRule::Usage::MANDATORY) {
            return "Checking list of subobjects for key " +
                    key +
                    ": No subobjects found at all.";
        } else {
            return "";
        }
    } else if (warelistrule.length != WarelistRule::lengthNotDefined
               && static_cast<label_t>(subData.second) != warelistrule.length) {
        return "Checking list of subobjects for key " +
                key +
                ": The required number of subobjects (" +
                Utility::toString(warelistrule.length) +
                ") does not match the actual number, length mismatch:" +
                Utility::toString(subData.second) +
                " inside the given CD.";
    }

    // Recusively check all subobject CDs.
    for (size_t i = 0; i < subData.second; ++i) {
        std::string error = subCV.check(subData.first->second);
        if (!error.empty())
            return "In subobject data for '" + key + "':\n  " + error;
        ++(subData.first);
    }
    // clang-format on
    return "";
}

} // namespace
///////////////////////////////////////////////////////////////////////////////////////////////////

std::string ConstructionValidator::checkSettingRuleKeyAndReturnErrors(ConstructionData const &cd, std::string const &key) const
{
    std::string error;
    auto keyList = cd.s.keys();
    auto cdKey = std::find(keyList.cbegin(), keyList.cend(), key);
    if (cdKey != keyList.cend()) {
        //for (auto const &cdKey : keyList) {
        if (cd.s.value(key).isValid() && !hasSettingRule(key)) {
            return error = "Construction data settings key '" + key + "' does not match any SettingRule key.";
        }
        //}
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
    auto const &wlr = warelistRules_.at(key);
    if (wlr.length != WarelistRule::lengthSingleton) {
        return "Warelistrule for key '" + key
               + "' refers to a subobject list and not for a single subobject. Call 'checkSubobjectListAndReturnErrors' instead.";
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
        return "There is no registered warelistrule for key '" + key + "'. Unable to validate the subobjects.";
    }

    auto const &wlr = warelistRules_.at(key);

    // Check if there are more then one subobjects building objects of the same type
    if (wlr.length == WarelistRule::lengthSingleton) {
        return "Warelistrule for key '" + key
               + "' refers to a single subobject and not a list. Call 'checkSubobjectAndReturnErrors' instead.";
    }

    // get the subobject's CV and call the recursive template check function.
    auto const &subobjectCV = wlr.callbackCV();
    return checkSubobjectListRecursevilyAndReturnErrors(cd.subObjectData, key, subobjectCV, wlr);
}

std::string ConstructionValidator::checkSharedWareAndReturnErrors(ConstructionData const &cd,
                                                                  std::string const &key,
                                                                  std::string const &referredTypeName) const
{
    if (!hasWarelistRule(key)) {
        return "There is no registered warelistrule for key '" + key + "'. Unable to validate the shared ware.";
    }
    auto const &warelistrule = warelistRules_.at(key);
    if (warelistrule.length != WarelistRule::lengthSingleton) {
        return "Warelistrule for key '" + key
               + "' refers to a shared ware list and not a single ware. Call 'checkSharedWareListAndReturnErrors' instead.";
    }
    return checkWareObjectAndReturnErrors(cd.sharedWares, key, referredTypeName, warelistrule);
}

std::string ConstructionValidator::checkSharedWareListAndReturnErrors(ConstructionData const &cd,
                                                                      std::string const &key,
                                                                      std::string const &referredTypeName) const
{
    if (warelistRules_.find(key) == warelistRules_.cend()) {
        return "There is no registered warelistrule for key '" + key + "'. Unable to validate the subobjects.";
    }
    auto const &warelistrule = warelistRules_.at(key);
    if (warelistrule.length == WarelistRule::lengthSingleton) {
        return "Warelistrule for key '" + key
               + "' refers to a single shared ware and not a list. Call 'checkSharedWareAndReturnErrors' instead.";
    }
    return checkWareListRecursivelyAndReturnErrors(cd.sharedWares, key, referredTypeName, warelistrule);
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

std::string ConstructionValidator::validateSharedWareAndReturnId(ConstructionData const &cd,
                                                                 std::string const &key,
                                                                 std::string const &referredTypeName) const
{
    std::string error = checkSharedWareAndReturnErrors(cd, key, referredTypeName);
    if (!error.empty())
        D_THROW(error);

    // Return named reference ID or empty string if there is no registered ware.´
    auto variantWare = cd.wareSettings.value(key + ConstructionData::seperator + Utility::toString(0));
    if (variantWare.isMonostate()) {
        return "";
    }
    return variantWare.toString();
}

std::shared_ptr<const Ware> ConstructionValidator::validateSharedWareAndReturnPointer(ConstructionData const &cd,
                                                                                      std::string const &key,
                                                                                      std::string const &referredTypeName) const
{
    std::string error = checkSharedWareAndReturnErrors(cd, key, referredTypeName);
    if (!error.empty())
        D_THROW(error);
    // Return the corresponding shared pointer inside construction data's shared ware map.
    // It was added during call of 'ConstructionData::addSharedWare'.
    // Remember, here only single shared wares are treated and no lists.
    auto result = cd.sharedWares.find(key + ConstructionData::seperator + Utility::toString(0));
    if (result != cd.sharedWares.cend()) {
        return result->second;
    }
    return nullptr;
}

std::vector<std::pair<std::string, std::shared_ptr<const Ware>>> ConstructionValidator::validateSharedListAndReturnValues(
    ConstructionData const &cd, std::string const &key, std::string const &referredTypeName) const
{
    std::string error = checkSharedWareAndReturnErrors(cd, key, referredTypeName);
    if (!error.empty())
        D_THROW(error);

    std::vector<std::pair<std::string, std::shared_ptr<const Ware>>> result;
    for (label_t count = 0;; ++count) {
        auto keyString = key + ConstructionData::seperator + std::to_string(count);
        auto wIter = cd.sharedWares.find(keyString);
        if (wIter == cd.sharedWares.end())
            break;
        result.push_back(std::make_pair(cd.s.value(keyString).toString(), wIter->second));
    }
    return result;
}
} // namespace DUTIL
