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
             SR::Usage::OPTIONAL, "Start time of the clock as a value of time since epoche.");
         //sr.defaultValue = Clock::resetStart();
         return sr;
       }()},
      {});
  return cv;
}

Clock::Clock() :
    clock_(std::chrono::system_clock()),
    type_(Clock::Type::SYSTEM),
    zone_(Zone::LOCAL)
{
  initialize();
}

Clock::Clock(ConstructionData const& cd) :
    type_(getConstructionValidator().validateNamedEnum<Clock::Type>(cd)),
    zone_(getConstructionValidator().validateNamedEnum<Clock::Zone>(cd))
{

  switch (type_) {
    case Clock::Type::SYSTEM:
      clock_ = std::chrono::system_clock();
      break;
    case Clock::Type::STEADY:
      clock_ = std::chrono::steady_clock();
      break;
    case Clock::Type::HIGH_RESOLUTION:
      clock_ = std::chrono::high_resolution_clock();
      break;
  }
  initialize();
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
  resetStart();
  advance();
}

void Clock::advance()
{
  std::visit(
      Overload{
          [&](std::chrono::system_clock) {
            if (type_ == Type::SYSTEM) {
              intermediate_ = std::chrono::system_clock::now();
            } else {
              D_ASSERT(type_ == Type::HIGH_RESOLUTION);
              intermediate_ = std::chrono::high_resolution_clock::now();
            }
          },
          [&](std::chrono::steady_clock) { intermediate_ = std::chrono::steady_clock::now(); },
      },
      clock_);
}

TIME::nano_sec_t Clock::elapsed(TIME::UnitPrefix prefix) const
{
  // check if starting and intermediate time point variant have the same index.
  D_ASSERT(start_.index() == intermediate_.index());

  auto elapsed = std::visit(
      Overload{[&](std::chrono::system_clock) {
                 auto tp1 = getStartTimePoint<std::chrono::system_clock::time_point>();
                 auto tp2 = getIntermediateTimePoint<std::chrono::system_clock::time_point>();
                 return std::chrono::duration_cast<std::chrono::nanoseconds>(tp2 - tp1);
               },
               [&](std::chrono::steady_clock) {
                 auto tp1 = getStartTimePoint<std::chrono::steady_clock::time_point>();
                 auto tp2 = getIntermediateTimePoint<std::chrono::steady_clock::time_point>();
                 return std::chrono::duration_cast<std::chrono::nanoseconds>(tp2 - tp1);
               }},
      clock_);

  switch (prefix) {
    case TIME::UnitPrefix::NANO:
      return elapsed.count();
      break;
    case TIME::UnitPrefix::MICRO:
      return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
      break;
    case TIME::UnitPrefix::MILLI:
      return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
      break;
    case TIME::UnitPrefix::SECONDS:
      return std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
      break;
    case TIME::UnitPrefix::KILO:
      return static_cast<TIME::basic_sec_t>(
          std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() * TIME::oneByKilo);
      break;
    case TIME::UnitPrefix::MEGA:
      return static_cast<TIME::basic_sec_t>(
          std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() * TIME::oneByMega);
      break;
    default:
      return elapsed.count();
      break;
  }
}

void Clock::resetStart()
{
  std::visit(Overload{
                 [&](std::chrono::system_clock) {
                   if (type_ == Type::SYSTEM) {
                     start_ = std::chrono::system_clock::now();
                   } else {
                     D_ASSERT(type_ == Type::HIGH_RESOLUTION);
                     start_ = std::chrono::high_resolution_clock::now();
                   }
                 },
                 [&](std::chrono::steady_clock) { start_ = std::chrono::steady_clock::now(); },
             },
             clock_);
}

TIME::basic_sec_t Clock::getStartTimeSinceEpoche(TIME::UnitPrefix prefix) const
{
  auto elapsed
      = std::visit(Overload{[&](std::chrono::system_clock) {
                              auto tp = getStartTimePoint<std::chrono::system_clock::time_point>();
                              return tp.time_since_epoch();
                            },
                            [&](std::chrono::steady_clock) {
                              auto tp = getStartTimePoint<std::chrono::steady_clock::time_point>();
                              return tp.time_since_epoch();
                            }},
                   clock_);

  switch (prefix) {
    case TIME::UnitPrefix::NANO:
      return elapsed.count();
      break;
    case TIME::UnitPrefix::MICRO:
      return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
      break;
    case TIME::UnitPrefix::MILLI:
      return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
      break;
    case TIME::UnitPrefix::SECONDS:
      return std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
      break;
    case TIME::UnitPrefix::KILO:
      return static_cast<TIME::basic_sec_t>(
          std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() * TIME::oneByKilo);
      break;
    case TIME::UnitPrefix::MEGA:
      return static_cast<TIME::basic_sec_t>(
          std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() * TIME::oneByMega);
      break;
    default:
      return elapsed.count();
      break;
  }
}
