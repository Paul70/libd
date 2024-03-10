#include "ticker.h"

using namespace DUTIL;

D_DEFINE_PROJECTWARE(Ticker)

ConstructionValidator const& Ticker::getConstructionValidator()
{
  using SR = SettingRule;
  static const ConstructionValidator cv(
      {[]() {
         auto sr = SR::forNamedParameter<Ticker::Resolution_s>(
             SR::Usage::MANDATORY_WITH_DEFAULT,
             "Define the factor between ticker ticks and real time in seconds.\n"
             "A Resolution_s of 1 implies that the tick counter value corresponds to "
             "multiple of seconds.\n"
             "A Resolution_s of 0.001 implies that the tick counter value corresponds to "
             "multiples of miliseconds.");
         sr.defaultValue = 1.0;
         return sr;
       }(),
       []() {
         SR sr = SR::forNamedParameter<Ticker::StartTick>(
             SR::Usage::MANDATORY_WITH_DEFAULT,
             "Define the start time tick. A time tick is an integer counter which will be"
             "increased every time the ticker advnances in time by any step. A time tick "
             "corresponds to a real time point.");
         sr.defaultValue = 0;
         return sr;
       }()},
      {});
  return cv;
}

Ticker::Ticker() :
    now_(0),
    start_(0),
    resolution_s(1.0)
{}

Ticker::Ticker(ConstructionData const& cd) :
    now_(getConstructionValidator().validateNamedParameter<Ticker::StartTick>(cd)),
    start_(getConstructionValidator().validateNamedParameter<Ticker::StartTick>(cd)),
    resolution_s(getConstructionValidator().validateNamedParameter<Ticker::Resolution_s>(cd))
{}

real_t Ticker::getResolution_s() const
{
  return resolution_s;
}

Ticker::Tick Ticker::advance(Tick ticks)
{
  now_ = ticks;
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
