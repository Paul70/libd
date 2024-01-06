#ifndef DUTIL_NOW_H
#define DUTIL_NOW_H
#include "concretefactory.h"
#include "namedclass.h"
#include "namedenum.h"
#include "projectware.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <variant>

namespace DUTIL {
class ConstructionValidator;
struct ConstructionData;

/*! \brief Set and save the current time point depending on the
 *  specified clock.
 *
 *  Supported clock types are all C11 clocks: std::chrono::system_clock and
 *  std::chrono::steady_clock. Please note, that std::chrono::high_resolution_clock
 *  is an alias of std::chrono::system_clock. Using std::chrono::high_resolution_clock
 *  is just fine, however, programmatical it is a std::chrono::system_clock.
 */

class Now : public ProjectWare, public D_NAMED_CLASS(::DUTIL::Now)
{
  public:
  using Tick = std::uint64_t;

  D_DECLARE_PROJECTWARE(Now);

  //! Enum listing supported clock types.
  D_NAMED_ENUM(Clock, SYSTEM, STEADY, HIGH_RESOLUTION)

  //! Enum specifying the time zone for the time point.
  D_NAMED_ENUM(Zone, UTC, LOCAL)

  //! An integer tick counter.
  D_NAMED_PARAMETER(TickParam, std::uint64_t)

  //! Return current time point.
  template <typename Clock = std::chrono::system_clock>
  static std::chrono::time_point<Clock> now()
  {
    Clock::now();
  }

  /*! \brief Put a time point into a std stream object.
   *
   * Function does not change the tick and time point owned
   * by instances of this class.
   */
  template <typename S = std::ostream, typename C = std::chrono::system_clock>
  static void putToStream(S& stream = std::cout, Zone z = Zone::LOCAL)
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

  //! Default constructor.
  Now();

  //! Constructor with custom settings using ConstructionData.
  Now(ConstructionData const& cd);

  /*! \brief Advance by one time tick.
   *
   * Update saved time point to now and increase the time tick
   * parameter.
   */
  Tick advance();

  template <typename Clock>
  std::pair<Tick, std::chrono::time_point<Clock>> getNow() const
  {
    return std::make_pair(tick_, std::get<std::chrono::time_point<Clock>>(timepoint_));
  }

  /*! \brief Put saved time point into a std stream object.
   *
   * Uses the saved time point, that is this method my treats a time
   * point from the past.
   */
  template <typename T = std::ostream, typename Clock = std::chrono::system_clock>
  void putSavedToStream(T& stream = std::cout) const
  {
    auto tp = std::get<std::chrono::time_point<Clock>>(timepoint_);
    const std::time_t t_c = Clock::to_time_t(tp);
    if (zone_ == Zone::LOCAL)
      stream << std::put_time(std::localtime(&t_c), "%F %T");
    else
      stream << std::put_time(std::gmtime(&t_c), "%F %T");
  }

  private:
  using TimePoint = std::variant<std::chrono::time_point<std::chrono::system_clock>,
                                 std::chrono::time_point<std::chrono::steady_clock>>;

  void setTimePoint();

  TimePoint timepoint_;
  Tick tick_;
  const Clock clock_;
  const Zone zone_;
};

}  // namespace DUTIL
#endif  // DUTIL_NOW_H
