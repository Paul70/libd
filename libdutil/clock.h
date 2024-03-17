#ifndef DUTIL_CLOCK_H
#define DUTIL_CLOCK_H
#include "concretefactory.h"
#include "namedclass.h"
#include "projectware.h"

#include <chrono>
#include <iomanip>
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
  D_NAMED_PARAMETER(StartTime, TIME::basic_sec_t);

  /*! \brief Put a time point into a std stream object.
   *
   * Function does not change the tick and time point owned
   * by instances of this class. This method uses a std::chrono::system_clock.
   */
  template <typename S = std::ostream>
  static void putNowToStream(S& stream = std::cout, Zone z = Zone::LOCAL)
  {
    std::time_t t_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

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
  DUTIL::Clock::Type getType() const;

  /*! \brief Sets both the starting and intermediate time point to now.
   *
   * Determines now due to the specified clock type by calling the
   * clock's now() method.
   * Method does nothing if start time point is already set.
   */
  void initialize();

  //! Resets the start time point to now.
  void resetStart();

  //! Return the start time as absolute number since start of epoche.
  TIME::basic_sec_t getStartTimeSinceEpoche(TIME::UnitPrefix prefix = TIME::UnitPrefix::NANO) const;

  /*! \brief Set the intermediate time point to the current clock time.
   *
   * Determines now due to the specified clock type by calling the
   * clock's now() method.
   */
  void advance();

  /*! \brief Determines the duration between starting and intermediate
   *  time point in milliseconds.
   *
   * Asserts in case the clock object is not initialized.
   * Calculation internally is based on nanoseconds
   */
  TIME::basic_sec_t elapsed(TIME::UnitPrefix prefix = TIME::UnitPrefix::NANO) const;

  TIME::milli_sec_t timeSinceEpoch_milli_sec() const;

  template <typename T>
  T getStartTimePoint() const
  {
    return std::get<T>(start_);
  }

  template <typename T>
  T getIntermediateTimePoint() const
  {
    return std::get<T>(intermediate_);
  }

  /*! \brief Put saved time point into a std stream object.
   *
   * Uses the saved time point, that is this method my treats a time
   * point from the past.
   */
  template <typename T = std::ostream>
  void putIntermediateTimePointToStream(T& stream = std::cout) const
  {
    std::visit(
        Overload{
            [&](std::chrono::system_clock) {
              auto tp = getIntermediateTimePoint<std::chrono::system_clock::time_point>();
              const std::time_t t_c = std::chrono::system_clock::to_time_t(tp);
              if (zone_ == Zone::LOCAL)
                stream << std::put_time(std::localtime(&t_c), "%F %T");
              else
                stream << std::put_time(std::gmtime(&t_c), "%F %T");
            },
            [&](std::chrono::steady_clock) {
              D_THROW("Putting a std::steady_clock time point into a stream is not supported.");
            },
        },
        clock_);
  }

  private:
  //! Type alias definitions.
  using ClockObject = std::variant<std::chrono::system_clock, std::chrono::steady_clock>;
  using TimePoint = std::variant<std::chrono::time_point<std::chrono::system_clock>,
                                 std::chrono::time_point<std::chrono::steady_clock>>;

  ClockObject clock_;
  const Type type_;
  const Zone zone_;

  TimePoint intermediate_;
  TimePoint start_;
};

}  // namespace DUTIL
#endif  // DUTIL_CLOCK_H
