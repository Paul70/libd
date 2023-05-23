#include "exception.h"

namespace DUTIL {
Exception::Exception(std::string const& message) :
    std::runtime_error(message)
{}
} // namespace DUTIL
