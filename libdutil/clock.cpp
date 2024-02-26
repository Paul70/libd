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
    timepoint_(),
    startTime_ms_(setStartTimeToNow())
{}

Clock::Clock(ConstructionData const& cd) :
    type_(getConstructionValidator().validateNamedEnum<Clock::Type>(cd)),
    zone_(getConstructionValidator().validateNamedEnum<Clock::Zone>(cd)),
    startTime_ms_(getConstructionValidator().validateNamedParameter<Clock::StartTime>(cd))
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
  // sets the time point to the current clock time
  advance();

  // defines the value for the starting time in milli seconds
  std::visit(Overload{[&](std::chrono::time_point<std::chrono::system_clock> tp) {
                        startTime_ms_ = tp.time_since_epoch().count();
                      },
                      [&](std::chrono::time_point<std::chrono::steady_clock> tp) {
                        startTime_ms_ = tp.time_since_epoch().count();
                      }},
             timepoint_);
}

TIME::milli_sec_t Clock::getStartTime_ms() const
{
  return startTime_ms_;
}

void Clock::advance()
{
  setTimePoint();
}

void Clock::setTimePoint()
{
  std::visit(Overload{
                 [&](std::chrono::system_clock) {
                   if (type_ == Type::SYSTEM)
                     timepoint_ = forSystemOrHighResolutionType_Now();
                 },
                 [&](std::chrono::steady_clock c) { timepoint_ = c.now(); },
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
