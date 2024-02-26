#include "ticker.h"

using namespace DUTIL;

D_DEFINE_PROJECTWARE(Ticker)

ConstructionValidator const& Ticker::getConstructionValidator()
{
  using SR = SettingRule;
  static const ConstructionValidator cv(
      {[]() {
        SR sr = SR::forNamedParameter<Ticker::StartTick>(
            SR::Usage::MANDATORY_WITH_DEFAULT,
            "Define the start time tick. A time tick is an integer counter which will be"
            "increased every time the simulation advnances in time by any step. A time tick "
            "corresponds to a real time point.");
        sr.defaultValue = 0;
        return sr;
      }()},
      {});
  return cv;
}

Ticker::Ticker() :
    now_(0),
    start_(0)
{}

Ticker::Ticker(ConstructionData const& cd) :
    now_(getConstructionValidator().validateNamedParameter<Ticker::StartTick>(cd)),
    start_(getConstructionValidator().validateNamedParameter<Ticker::StartTick>(cd))
{}

Ticker::Tick Ticker::advance(Tick ticks)
{
  now_ += ticks;
  return now_;
}

Ticker::Tick Ticker::now() const
{
  return now_;
}

Ticker::Tick Ticker::getStartTick() const
{
  return start_;
}
