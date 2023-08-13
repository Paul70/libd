#ifndef DUTIL_TIME_H
#define DUTIL_TIME_H
#include "namedenum.h"
#include <chrono>
#include <iomanip>

namespace DUTIL {
using namespace std::literals;

/*! \brief Time class for a more easy usage of std::chrono
 *  and std::ctime time packages.
 *
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

    //! Update current time point to now.
    TimePoint now()
    {
        now_ = Clock::now();
        return now_;
    }

    //! Put the time point in a std::stream.
    void toStream(std::ostream &os)
    {
        const std::time_t t_c = Clock::to_time_t(now_);
        switch (z_) {
        case Zone::LOCAL:
            os << std::put_time(std::localtime(&t_c), "%F %T");
            break;
        case Zone::UTC:
            os << std::put_time(std::gmtime(&t_c), "%F %T");
            break;
        }
    }

    //! Set a new time zone
    void changeZone(Zone timezone)
    {
        z_ = timezone;
    }

private:
    Zone z_;
    TimePoint now_;
};

} // namespace DUTIL
#endif // DUTIL_TIME_H
