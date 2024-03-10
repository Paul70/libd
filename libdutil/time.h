#ifndef DUTIL_TIME_H
#define DUTIL_TIME_H
#include "basictypes.h"

#include <cstdint>

namespace DUTIL {
namespace TIME {

//! Time unit definitions
using sec_t = std::int64_t;
using milli_sec_t = std::int64_t;
using nano_sec_t = std::int64_t;

//! Definition of converting factors.
static constexpr BasicTypes::real_t sByMs = 1e03;
static constexpr BasicTypes::real_t sByNs = 1e09;
static constexpr BasicTypes::real_t MsByNs = 1e06;
static constexpr BasicTypes::real_t NsByMs = 1e-06;

}  // namespace TIME

}  // namespace DUTIL
#endif  // DUTIL_TIME_H
