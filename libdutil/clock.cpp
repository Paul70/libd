#include "clock.h"
#include "overload.h"

using namespace DUTIL;

D_DEFINE_PROJECTWARE(Clock)

ConstructionValidator const& Clock::getConstructionValidator()
{
  using SR = SettingRule;
  static ConstructionValidator cv(
      {[]() {
         SR sr = SR::forNamedEnum<Clock::Type>(
             SR::Usage::MANDATORY_WITH_DEFAULT,
             "Define clock type, default is std::chrono::system_clock.");
         sr.defaultValue = toString(Clock::Type::SYSTEM);
         return sr;
       }(),
       []() {
         SR sr = SR::forNamedEnum<Clock::Zone>(SR::Usage::MANDATORY_WITH_DEFAULT,
                                               "Set time zone, default is local system time zone.");
         sr.defaultValue = toString(Clock::Zone::LOCAL);
         return sr;
       }(),
       []() {
         SR sr = SR::forNamedParameter<Clock::InitializeStartTimeWithSystemClock>(
             SR::Usage::MANDATORY_WITH_DEFAULT,
             "If set to true the simulation will report wall clock time.\n"
             "See the explanation for StartTime_ms for more details.");
         sr.defaultValue = true;
         return sr;
       }(),
       []() {
         SR sr = SR::forNamedParameter<Clock::StartTime>(
             SR::Usage::MANDATORY_WITH_DEFAULT, "Start time of the clock in milli seconds.");
         sr.defaultValue = Clock::setStartTimeToNow();
         return sr;
       }()},
      {});
  return cv;
}

Clock::Clock() :
    clock_(std::chrono::system_clock()),
    type_(Clock::Type::SYSTEM),
    zone_(Zone::LOCAL),
    intermediate_(),
    start_()
{
  initialize();
}

Clock::Clock(ConstructionData const& cd) :
    type_(getConstructionValidator().validateNamedEnum<Clock::Type>(cd)),
    zone_(getConstructionValidator().validateNamedEnum<Clock::Zone>(cd))
{
  setTimePoint();
}

Clock::Zone Clock::getTimeZone() const
{
  return zone_;
}

Clock::Type Clock::getType() const
{
  return type_;
}

void Clock::initialize()
{
  // sets the start and intermediate time point to the current clock time
  std::visit(Overload{[&](std::chrono::time_point<std::chrono::system_clock> tp) {
                        if (tp.time_since_epoch() == tp.min().time_since_epoch())
                          return;
                      },
                      [&](std::chrono::time_point<std::chrono::steady_clock> tp) {
                        if (tp.time_since_epoch() == tp.min().time_since_epoch())
                          return;
                      }},
             start_);
  setStartTimePoint();
  advance();
}

void Clock::advance()
{
  setTimePoint();
}

TIME::milli_sec_t Clock::elapsedMilliSec() const
{
  return std::chrono::milliseconds(elapsedNanoSec()).count();
}

TIME::nano_sec_t Clock::elapsedNanoSec() const
{
  // check if starting and intermediate time point variant have the same index.
  D_ASSERT(start_.index() == intermediate_.index());

  TIME::nano_sec_t elapsed;
  std::visit(
      Overload{[&](std::chrono::system_clock) {
                 auto tp1 = getStartTimePoint<std::chrono::system_clock::time_point>();
                 auto tp2 = getIntermediateTimePoint<std::chrono::system_clock::time_point>();
                 elapsed = (tp2 - tp1).count();
                 return;
               },
               [&](std::chrono::steady_clock) {
                 auto tp1 = getStartTimePoint<std::chrono::steady_clock::time_point>();
                 auto tp2 = getIntermediateTimePoint<std::chrono::steady_clock::time_point>();
                 elapsed = (tp2 - tp1).count();
                 return;
               }},
      clock_);

  return elapsed;
}

void Clock::setTimePoint()
{
  std::visit(Overload{
                 [&](std::chrono::system_clock) {
                   if (type_ == Type::SYSTEM)
                     intermediate_ = forSystemOrHighResolutionType_Now();
                 },
                 [&](std::chrono::steady_clock c) { intermediate_ = c.now(); },
             },
             clock_);
}

void Clock::setStartTimePoint()
{
  std::visit(Overload{
                 [&](std::chrono::system_clock) {
                   if (type_ == Type::SYSTEM)
                     start_ = forSystemOrHighResolutionType_Now();
                 },
                 [&](std::chrono::steady_clock c) { start_ = c.now(); },
             },
             clock_);
}

Clock::TimePoint Clock::forSystemOrHighResolutionType_Now() const
{
  if (type_ == Clock::Type::HIGH_RESOLUTION) {
    return std::chrono::system_clock::now();
  }
  return std::chrono::high_resolution_clock::now();
}
