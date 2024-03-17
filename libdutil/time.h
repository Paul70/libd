#ifndef DUTIL_TIME_H
#define DUTIL_TIME_H
#include "basictypes.h"
#include "namedenum.h"

#include <cstdint>

namespace DUTIL {
namespace TIME {

//! Time unit prefixes.
D_NAMED_ENUM(UnitPrefix, NANO, MICRO, MILLI, SECONDS, KILO, MEGA)

//! Time unit definitions
using basic_sec_t = std::int64_t;
using mega_sec_t = basic_sec_t;
using kilo_sec_t = basic_sec_t;
using sec_t = basic_sec_t;
using milli_sec_t = basic_sec_t;
using micro_sec_t = basic_sec_t;
using nano_sec_t = basic_sec_t;

//! Definition of converting factors.
static constexpr BasicTypes::real_t oneByMilli = 1e03;
static constexpr BasicTypes::real_t oneByMicro = 1e06;
static constexpr BasicTypes::real_t oneByNano = 1e09;
static constexpr BasicTypes::real_t oneByKilo = 1e-03;
static constexpr BasicTypes::real_t oneByMega = 1e-06;
static constexpr BasicTypes::real_t milliByNano = 1e06;
static constexpr BasicTypes::real_t nanoByMilli = 1e-06;
static constexpr BasicTypes::real_t nanoByOne = 1e-09;

}  // namespace TIME

}  // namespace DUTIL
#endif  // DUTIL_TIME_H
