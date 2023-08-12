#ifndef DUTIL_TIME_H
#define DUTIL_TIME_H
#include "namedenum.h"
#include <chrono>
#include <iomanip>

namespace DUTIL {
using namespace std::literals;

/*! \brief description of TimePoint
 *
 * Longer description of TimePoint.
 */

template<typename Clock>
class Time
{
public:
    using TimePoint = typename Clock::time_point;

    //! Enum specifying the time zone for the time point.
    D_NAMED_ENUM(Zone, UTC, LOCAL)

    //! Default construct.
    Time() :
        z_(Zone::LOCAL),
        now_(Clock::now())
    {}

    //! Constructor with custom settings.
    Time(Zone z) :
        z_(z),
        now_(Clock::now())
    {}

    TimePoint now()
    {
        now_ = Clock::now();
        return now_;
    }

    auto putToStream() const
    {
        const std::time_t t_c = Clock::to_time_t(now_);
        switch (z_) {
        case Zone::LOCAL:
            return std::put_time(std::localtime(&t_c), "%F %T");
        case Zone::UTC:
            return std::put_time(std::localtime(&t_c), "%F %T");
        default:
            return std::put_time(std::localtime(&t_c), "%F %T");
        }
    }

private:
    Zone z_;
    TimePoint now_;
};

} // namespace DUTIL
#endif // DUTIL_TIME_H
