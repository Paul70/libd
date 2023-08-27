#include "constructionvalidator.h"
#include "constructiondata.h"
#include "exception.h"

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
} // namespace

ConstructionValidator::ConstructionValidator() :
    settingRules_(),
    check_(trivialCheck)
{}

ConstructionValidator::ConstructionValidator(std::vector<SettingRule> settingRules,
                                             [[maybe_unused]] std::vector<WarelistRule> warelistRules,
                                             ConstructionValidator baseCV,
                                             CheckFunction checkF) :
    settingRules_(baseCV.settingRules_),
    warelistRules_(baseCV.warelistRules_),
    check_(baseCV.check_)
{
    for (auto const &sr : settingRules) {
        settingRules_.emplace(sr.key, checkSettingRule(sr));
    }
    //for (auto const &wr : warelistRules) {
    //warelistRules.emplace(wr.key, checkSettingRule(wr));
    //}

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
    // this function gets called inside the newInstance factory function! See the lambda in the constructor.
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
                + Utility::toStr(sr.minimalStringLength) + ".";
        return Variant();
    }
    // clang-format on
    return value;
}

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

std::string ConstructionValidator::checkSubObjectAndReturnErrors(ConstructionData const &, std::string const &) const
{
    std::string error;
    return error;
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
    static const ConstructionData subObjectCD;
    std::string error = checkSubObjectAndReturnErrors(cd, key);
    if (!error.empty()) {
        D_THROW(error);
    }
    auto result = cd.subObjectData.find(key + ConstructionData::seperator + "0");
    if (result != cd.subObjectData.cend()) {
        return result->second;
    } else {
        return subObjectCD;
    }
}

std::string ConstructionValidator::recursiveCheck(ConstructionValidator const &cv, ConstructionData const &cd)
{
    // check all available setting rules
    std::string errors;
    for (auto const &sr : cv.settingRules_) {
        errors = cv.checkSettingRuleKeyAndReturnErrors(cd, sr.first);
        if (!errors.empty()) {
            return errors;
        }
    }

    if (!errors.empty())
        D_THROW("At this point, construction validator error string has to be empty.");
    return errors;
}
} // namespace DUTIL
