#include "constructionvalidator.h"
#include "constructiondata.h"
#include "exception.h"

namespace DUTIL {

namespace {
bool checkASmallerThanB(Variant::Type type, Variant const &A, Variant const &B)
{
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
    // This function checks if the SettingRule itself is valid and not if ConstructionData settings
    // follow the setting rule.

    // do not continue if the setting rule does not fulfill all
    // requirements listed here.
    // - SettingRule key has to be set.
    // - a default value is set if usage == MANDATORY_WITH_DEFAULT or default value is in std::monostate if not.
    // - if there is a non emtpy list of possible values, SettingRule::Type value has to be string.
    // - if there is a valid min/max value, SettingRule::Type value has to be numeric and
    //   the conversion of min/max into SettingRule::Type must be valid
    // - the minimal value has to be smaller then the maximal value.

    D_ASSERT(!sr.key.empty());
    D_ASSERT(sr.defaultValue.isValid() || sr.usage != SettingRule::Usage::MANDATORY_WITH_DEFAULT);
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
        D_ASSERT(checkASmallerThanB(sr.type, sr.minimalValue, sr.maximalValue));
    }
    // ist hier noch nicht fertig
    // xxx presence in list xxx
    return sr;
}
} // namespace

ConstructionValidator::ConstructionValidator() :
    settingRules_(),
    check_(trivialCheck)
{}

ConstructionValidator::ConstructionValidator(std::vector<SettingRule> settingRules,
                                             ConstructionValidator baseCV,
                                             CheckFunction checkF) :
    settingRules_(baseCV.settingRules_),
    check_(baseCV.check_)
{
    for (auto const &sr : settingRules) {
        settingRules_.emplace(sr.key, checkSettingRule(sr));
    }
    if (checkF) {
        // assign a lambda to the check funtor
        // lambda first calls the check-function (not directly the functor) of the base construction validator.
        // In case of no errors, the custom check functor of this validator gets called.
        check_ = [=](ConstructionValidator const &cvThis, ConstructionData const &cdThis) {
            auto errors = check_(baseCV, cdThis);
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

SettingRule ConstructionValidator::getSettingRule(std::string const &key) const
{
    auto rule = settingRules_.find(key);
    if (rule == settingRules_.end()) {
        D_THROW("No SettingRule found for given key '" + key + "'.");
    }
    return rule->second;
}

Variant ConstructionValidator::checkSettingRuleKeyAndReturnValue(Variant const value,
                                                                 std::string const &key,
                                                                 std::string &error) const
{
    auto sr = getSettingRule(key);

    // check usage settings
    if (sr.usage == SettingRule::Usage::MANDATORY_WITH_DEFAULT && sr.defaultValue.isMonostate()) {
        error = "Setting rule for key '" + key + "' and value: " + value.toString() + "requires a default value.";
        return Variant();
    }
    if (sr.usage == SettingRule::Usage::MANDATORY_NO_DEFAULT && sr.defaultValue.isValid()) {
        error = "Setting rule for key '" + key + "' and value: " + value.toString()
                + "is mandatory and cannot rely on a default value.";
        return Variant();
    }

    // ckeck Variant type
    if (value.getType() != sr.type) {
        error = "Setting rule for key '" + key + "' and value: " + value.toString()
                + "defines a different type for the parameter";
        return Variant();
    }

    // check list of possible values
    if (!checkPresenceInListOfAllowedValues(sr.listOfPossibleValues, value.toString())) {
        error = "Setting rule for key '" + key + "' allows the following values: ";
        for (auto const &it : sr.listOfPossibleValues) {
            error += it + ",";
        }
        error += ". '" + value.toString() + "' is not in that list.";
        return Variant();
    }

    // check min/max limits
    if (!checkASmallerThanB(sr.type, sr.minimalValue, value)) {
        error = "Setting for key '" + key + "' and value: " + value.toString() + "is smaller than the allowed min value.";
        return Variant();
    }
    if (!checkASmallerThanB(sr.type, value, sr.maximalValue)) {
        error = "Setting for key '" + key + "' and value: " + value.toString() + "is bigger than the allowed max value.";
        return Variant();
    }

    // check max string length
    if (sr.type == Variant::Type::STRING && label_t(value.toString().size()) < sr.minimalStringLength) {
        error = "Setting for key '" + key + "' and value: " + value.toString() + "requires a min string length of "
                + Utility::toStr(sr.minimalStringLength) + ".";
        return Variant();
    }
    return value;
}

std::string ConstructionValidator::checkSettingRuleKeyAndReturnErrors(ConstructionData const &cd, std::string const &key) const
{
    std::string errors;
    checkSettingRuleKeyAndReturnValue(cd.s.value(key), key, errors);
    return errors;
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

    D_ASSERT_MSG(!errors.empty(), "At this point, error string has to be empty.");
    return errors;
}
} // namespace DUTIL
