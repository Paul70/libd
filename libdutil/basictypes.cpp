#include "basictypes.h"
#include "exception.h"
#include "utility.h"
#include <limits>

namespace DUTIL {
namespace BasicTypes {

void d_check_size(std::uint64_t size)
{
    if (size >= std::uint64_t(std::numeric_limits<label_t>::max()))
        D_THROW("container size " + Utility::toString(size) + " is too large for type label");
}
} // namespace BasicTypes
} // namespace DUTIL
