#ifndef DUTIL_TICKER_H
#define DUTIL_TICKER_H
#include "concretefactory.h"
#include "namedclass.h"
#include "namedparameter.h"
#include "projectware.h"

namespace DUTIL {
class ConstructionValidator;
struct ConstructionData;

/*! \brief description of Ticker
 *
 * Longer description of Ticker.
 */

class Ticker final : public ProjectWare, public D_NAMED_CLASS(::DUTIL::Ticker)
{
  public:
  //! Define typ for one single tick in time or tick in progress.
  using Tick = std::uint64_t;

  //! A single tick.
  static constexpr std::uint8_t single{1};

  //! Static factory member.
  D_DECLARE_PROJECTWARE(Ticker)

  //! Parameter for holding a start tick value.
  D_NAMED_PARAMETER(StartTick, Tick);

  //! Set construction rules to be validated.
  static ConstructionValidator const& getConstructionValidator();

  //! Default constructor
  Ticker();

  //! Construct with construction data
  explicit Ticker(ConstructionData const& cd);

  Ticker(Ticker const& t) = default;
  Ticker(Ticker&& t) = default;

  //! Increase the ticker by the given value.
  Tick advance(Tick ticks = Ticker::single);

  //! Return the current tick value.
  Tick now() const;

  //! Return the start tick value.
  Tick getStartTick() const;

  private:
  Tick now_;
  const Tick start_;
};

}  // namespace DUTIL
#endif  // DUTIL_TICKER_H
