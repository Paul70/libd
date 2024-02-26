#ifndef DUTIL_NOW_H
#define DUTIL_NOW_H
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

class Now : public ProjectWare
{
  public:
  //! Define typ for one single tick in time or tick in progress.
  using Tick = std::uint64_t;

  //! Return current time point.
  template <typename Clock = std::chrono::system_clock>
  static std::chrono::time_point<Clock> now()
  {
    Clock::now();
  }

  /*! \brief Advance by one time tick.
   *
   * Update saved time point to now and increase the time tick
   * parameter.
   */
  //void advance(Tick ticks);

  template <typename Clock>
  std::pair<Tick, std::chrono::time_point<Clock>> getNow() const
  {
    //return std::make_pair(tick_, std::get<std::chrono::time_point<Clock>>(timepoint_));
  }

  private:
  virtual void advanceImpl(Tick ticks) = 0;

  void setTimePoint();
};

}  // namespace DUTIL
#endif  // DUTIL_NOW_H
