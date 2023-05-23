#include "utility.h"
#include <algorithm>
#include <array>
#include <charconv>
#include <iostream>

namespace DUTIL {
namespace Utility {

void replaceDecimalSeperator(char const oldSep, char const newSep, std::string& value) noexcept
{
    auto pos = value.find(oldSep);
    if(pos != std::string::npos) {
        value.replace(value.begin() + pos, value.begin() + pos + 1, 1, newSep);
    }
}

std::string integerToString(int value) noexcept
{
    // Largest number representable by a four byte int type is (2^31)-1 since one digit is reserved for the minus sign.
    // That corresponds to a 11 digit number including the decimal point and possible minus sign.
    std::array<char, 11> buffer{'\0'};
    std::to_chars(buffer.begin(), buffer.end(), value);
    return std::string(buffer.begin());
}

std::string integerToString(std::int64_t value) noexcept
{
    std::array<char, 21> buffer{'\0'};
    std::to_chars(buffer.begin(), buffer.end(), value);
    return std::string(buffer.begin());
}

std::string doubleToString(double value, int precision) noexcept
{
    if(precision > 20) {
        precision = 20;
        // hier eine log meldung
    }
    // Two additional digits for decimal seperator and minus sign.
    std::array<char, 22> buffer{'\0'};
    std::to_chars(buffer.begin(), buffer.end(), value, std::chars_format::fixed, precision);
    std::string num{buffer.begin()};
    return num;
}

StringList split(std::string const& str, char const seperator, bool removeWS)
{
    size_t next = str.find(seperator, 0);
    if(next == std::string::npos) {
        return StringList();
    }

    StringList list{};
    list.reserve(10);
    size_t last = 0;
    do {
        list.push_back(str.substr(last, next - last));
        last = ++next;
        next = str.find(seperator, next);
        if(removeWS) {
            while(list.back().back() == ' ') {
                list.back().pop_back();
            }
            while(list.back().front() == ' ') {
                list.back().erase(list.back().begin());
            }
        }
    } while(last != 0);
    return list;
}

} // namespace Utility
} // namespace DUTIL
