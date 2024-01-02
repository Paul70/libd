#ifndef DUTIL_UTILITY_H
#define DUTIL_UTILITY_H
#include "basictypes.h"
#include "exception.h"
#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace DUTIL {
using namespace BasicTypes;

/*! \brief Collection of useful functions
 *
 * Utility namespace is a collection of useful functions, macros etc.
 * in a general context.
 */
namespace Utility {

/*! \brief Change a decimal seperator.
 *
 * The decimal seperator in numeric strings depends on local system settings.
 * It is sometimes necessary to replace comma by period. The function does nothing
 * if no seperator is found.
 */
void replaceDecimalSeperator(char const oldSep, char const newSep, std::string &value) noexcept;

/*! \brief Convert an integer or double number into std::string.
 *
 * Functions use std::to_chars for conversion since it is more robust with
 * respect to std::locale settings.
 */
std::string integerToString(int value) noexcept;
std::string integerToString(std::int64_t value) noexcept;
std::string doubleToString(double value, int precision = 10) noexcept;

/*! \brief Convert an arithmetic type number into std::string.
 *
 * Transformation is only possible from std::arithmetic types.
 */
template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
std::string arithmeticToString(T value, std::int8_t precision = 10) noexcept
{
    if constexpr (is_integer_v<T>) {
        return integerToString(value);
    } else if constexpr (std::is_floating_point_v<T>) {
        return doubleToString(value, precision);
    } else {
        auto result = std::to_string(value);
        replaceDecimalSeperator(',', '.', result);
        return result;
    }
}

/*! \brief Convert a std::string into an arithmetic type.
 *
 * 1. Target type is integer:
 *    Uses std::stoll and casting for conversion to T type.
 *
 * 2. Target type is unsidned integer:
 *    Uses std::stoull and casting for conversion to T type. If string
 *    number is negative, the minus sign gets removed ant number is treated
 *    as an absolute values
 *
 * 3. Target type is double:
 *    Uses std::stod for conversion.
 *
 * 4. Target type is bool:
 *    Retruns true if string is not empty, otherwise false.
 *
 * 5. Target type char:
 *    Conversion of string into char is not supported since it does not make sense.
 *    An exception is thrown in that case.
 *
 * Throws exections in the following cases:
 * i)   If target type T is char which is not supported by this function
 * ii)  If string contains an invalid value which is not convertible by the above functions.
 * iii) If the string number is out of range of the target type.
 */
template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
T stringToArithmetic(std::string &value)
{
    Utility::replaceDecimalSeperator(',', '.', value);
    try {
        if constexpr (is_integer_v<T>) {
            return static_cast<T>(std::stoll(value));
        }
        if constexpr (std::is_unsigned_v<T>) {
            if (value.front() == '-')
                value.erase(value.begin());
            return std::stoull(value);
        }
        if constexpr (std::is_same_v<T, double>) {
            return std::stod(value);
        }
        if constexpr (std::is_same_v<T, bool>) {
            return !value.empty();
        }
        D_ASSERT_MSG(false, "Unimplemented case!");
    } catch (std::invalid_argument const &ex) {
        D_THROW(ex.what());
    } catch (std::out_of_range const &ex) {
        D_THROW(ex.what());
    }
}

/*! \brief Convert a given type T into std::string.
 *
 * Shortcut function for other ...ToString functions specified here.
 *
 * 1. Transformation of strings into arithmetic types follows arithmeticToString
 *    function.
 *
 * 2. Other types not implemented yet
 */
template<typename T>
std::string toString(T value)
{
    if constexpr (is_string_v<T>) {
        return value;
    }
    if constexpr (std::is_arithmetic_v<T>) {
        return arithmeticToString(value);
    }
    D_ASSERT_MSG(false, "Unimplemented case!");
    // later, if a conversion is not possible and it is no error, this function return an empty string
}

/*! \brief Convert a std::string into the given target type T.
 *
 * Shortcut function for other strintTo... functions specified here.
 *
 * 1. Arithmetic types int, uint, double, bool and char are converted according
 *    to stringToArithmetic fucntion.
 * 2. If target type is std::string just return the input value.
 *
 * 3. Other types not implemented yet
 */
template<typename T>
std::pair<bool, T> fromString(std::string &value)
{
    if constexpr (std::is_arithmetic_v<T>) {
        return std::make_pair(true, stringToArithmetic<T>(value));
    } else if constexpr (std::is_same_v<T, std::string>) {
        return std::make_pair(true, std::string(value));
    }
    D_ASSERT_MSG(false, "Unimplemented case!");
}

/*! \brief Remove leading and trailing whitespace characters from a string.
 *
 *  According to https://en.cppreference.com (std::isspace) whitespace characters are:
 *  - space (0x20, ' ')
 *  - form feed (0x0c, '\f')
 *  - line feed (0x0a, '\n')
 *  - carriage return (0x0d, '\r')
 *  - horizontal tab (0x09, '\t')
 *  - vertical tab (0x0b, '\v')
 *
 * \remarks Since overloading trim with paramter std::string and std::string&
 *          might have some dangerous side effects this funciotn is named
 *          trimThis
 */
void trimThis(std::string &str);
std::string trimMove(std::string &&str);
std::string trim(std::string str);

/*! \brief Remove trailing zero characters from a string.
 *
 * For example:
 *  "2.300000000" becomes "2.3"
 */
void trimZeros(std::string &str);

/*! \brief Split a single string into substrings.
 *
 * Dividence is based on the seperator character such as comma or semi colon.
 * The substrings will be put into a StringList which is an alias for
 * std::vecttor<std::string>. If the string does not contain the specified seperartor,
 * the function returns an empty list.
 *
 * Example:
 * "RED = 5, BLUE = 10, GREEN" will be seperated as StringList{"RED = 5","BLUE = 10","GREEN"}
 * Note, that leading and trailing whitespaces are removed by default using Utility::trim function.
 * If this behaviour is not wanted, set the flag 'removeWS' to false.
 */
StringList split(std::string const &str, char const seperator = ',', bool removeWS = true);

} // namespace Utility
} // namespace DUTIL
#endif // DUTIL_UTILITY_H
