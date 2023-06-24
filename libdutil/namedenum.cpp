#include "namedenum.h"
#include "exception.h"
#include "variant.h"

namespace DUTIL {
namespace NamedEnumDetail {

void dealWithExcpetion(std::string const msg)
{
    D_THROW(msg);
}

std::string variantToString(DUTIL::Variant const &variant)
{
    if(variant.isString()){
        return variant.toString();
    }
    else{
        D_THROW("Variant parameter does not hold a std::string value.");
        return std::string {};
    }
}
} //namespace NamedEnumDetail
} // namespace DUTIL
