#ifndef DUTIL_CLOCK_H
#define DUTIL_CLOCK_H
#include "concretefactory.h"
#include "namedclass.h"
#include "now.h"
#include "projectware.h"
#include "time.h"

namespace DUTIL {
class ConstructionValidator;
struct ConstructionData;

/*! \brief description of Clock
 *
 * Longer description of Clock.
 */

class Clock final : public ProjectWare, public D_NAMED_CLASS(::DUTIL::Clock)
{
  public:
  //! Static factory member.
  D_DECLARE_PROJECTWARE(Clock)

  //! Enum listing supported clock types.
  D_NAMED_ENUM(Type, SYSTEM, STEADY, HIGH_RESOLUTION)

  //! Enum specifying the time zone for the time point.
  D_NAMED_ENUM(Zone, UTC, LOCAL)

  //! If true, set StartTime_ms to system clock when the clock starts.
  D_NAMED_BOOL(InitializeStartTimeWithSystemClock)

  //! Set the start time value.
  D_NAMED_PARAMETER(StartTime, TIME::milli_sec_t);

  template <typename C = std::chrono::system_clock>
  static TIME::milli_sec_t setStartTimeToNow()
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(C::now().time_since_epoch())
        .count();
  }

  /*! \brief Put a time point into a std stream object.
   *
   * Function does not change the tick and time point owned
   * by instances of this class.
   */
  template <typename S = std::ostream, typename C = std::chrono::system_clock>
  static void putNowToStream(S& stream = std::cout, Zone z = Zone::LOCAL)
  {
    std::time_t t_c;
    if constexpr (std::is_same_v<C, std::chrono::system_clock>)
      t_c = std::chrono::system_clock::to_time_t(Now::now());
    else
      t_c = std::chrono::high_resolution_clock::to_time_t(
          Now::now<std::chrono::high_resolution_clock>());

    if (z == Zone::LOCAL)
      stream << std::put_time(std::localtime(&t_c), "%F %T");
    else
      stream << std::put_time(std::gmtime(&t_c), "%F %T");
  }

  //! Set construction rules.
  static ConstructionValidator const& getConstructionValidator();

  //! Default construction as system clock in the local time zone depending on operating system settings.
  Clock();

  //! Construct with construction data
  explicit Clock(ConstructionData const& cd);

  //! Return the time zone of this clock.
  Clock::Zone getTimeZone() const;

  //! Return the clock type.
  Clock::Type getType() const;

  //! Use the current time as start time.
  void initialize();

  //! Return the start time in milli seconds
  TIME::milli_sec_t getStartTime_ms() const;

  //! Set the time point to the current clock time.
  void advance();

  TIME::milli_sec_t timeSinceEpoch_milli_sec() const;

  /*! \brief Put saved time point into a std stream object.
   *
   * Uses the saved time point, that is this method my treats a time
   * point from the past.
   */
  template <typename T = std::ostream, typename Clock = std::chrono::system_clock>
  void putSavedTimePointToStream(T& stream = std::cout) const
  {
    auto tp = std::get<std::chrono::time_point<Clock>>(timepoint_);
    const std::time_t t_c = Clock::to_time_t(tp);
    if (zone_ == Zone::LOCAL)
      stream << std::put_time(std::localtime(&t_c), "%F %T");
    else
      stream << std::put_time(std::gmtime(&t_c), "%F %T");
  }

  private:
  //! Type alias definitions.
  using ClockObject = std::variant<std::chrono::system_clock, std::chrono::steady_clock>;
  using TimePoint = std::variant<std::chrono::time_point<std::chrono::system_clock>,
                                 std::chrono::time_point<std::chrono::steady_clock>>;

  void setTimePoint();
  TimePoint forSystemOrHighResolutionType_Now() const;

  const ClockObject clock_;
  const Type type_;
  const Zone zone_;
  TimePoint timepoint_;
  TIME::milli_sec_t startTime_ms_;
};

}  // namespace DUTIL
#endif  // DUTIL_CLOCK_H
