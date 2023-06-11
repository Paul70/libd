#ifndef DUTIL_EXCEPTION_H
#define DUTIL_EXCEPTION_H
#include <cassert>
#include <stdexcept>

namespace DUTIL {

/*! \brief description of Exception
 *
 * Longer description of Exception.
 */

class Exception : public std::runtime_error
{
public:
    explicit Exception(std::string const &message);
};
} // namespace DUTIL

//! Defien a macro to identify function names.
//! __PRETTY_FUNCTION__ is an implicitely declared variable by GCC. It contains the fucntion
//! name and its signature. MSVC does not support __PRETTY_FUNCTION__. In the MSVC case use
//! the pre-standard __FUNCTION__ identifier to obtain a fucntion name.
#ifdef D_GCC
#ifdef CXX_20
//mit dem std::source_location::function_name arbeiten aber mit dem gleichen macro name
#else
#define D_GET_FUNCTION_NAME() __PRETTY_FUNCTION__
#endif
#endif

//! Easy to use exception
#define D_THROW(message) \
    throw DUTIL::Exception(std::string("error happend in ") + D_GET_FUNCTION_NAME() \
                           + std::string(" ") + message)

//! Define easy to use custom assert macros
#ifdef D_DEBUG
#define D_ASSERT_MSG(condition, message) assert(((void) message, condition))
#define D_ASSERT(condition) assert(condition)
#else
#define D_ASSERT_MSG(condition, message) ((void) 0)
#define D_ASSERT(condition) ((void) 0)
#endif

#endif // DUTIL_EXCEPTION_H
