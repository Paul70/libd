#include "libdutil/now.h"
#include "libdutil/constructiondata.h"
#include "libdutil/constructionvalidator.h"

using namespace DUTIL;

D_DEFINE_PROJECTWARE(Now);

ConstructionValidator const& Now::getConstructionValidator()
{
  using SR = SettingRule;
  static ConstructionValidator cv(
      {[]() {
         SR sr = SR::forNamedEnum<Now::Clock>(
             SR::Usage::MANDATORY_WITH_DEFAULT,
             "Define clock type, default is std::chrono::system_clock.");
         sr.defaultValue = toString(Now::Clock::SYSTEM);
         return sr;
       }(),
       []() {
         SR sr = SR::forNamedEnum<Now::Zone>(SR::Usage::MANDATORY_WITH_DEFAULT,
                                             "Set time zone, default is local system time zone.");
         sr.defaultValue = toString(Now::Zone::LOCAL);
         return sr;
       }()},
      {});
  return cv;
}

Now::Now() :
    timepoint_(std::chrono::system_clock::now()),
    tick_(0),
    clock_(Now::Clock::SYSTEM),
    zone_(Now::Zone::LOCAL)
{}

Now::Now(ConstructionData const& cd) :
    timepoint_(),
    tick_(0),
    clock_(getConstructionValidator().validateNamedEnum<Now::Clock>(cd)),
    zone_(getConstructionValidator().validateNamedEnum<Now::Zone>(cd))
{
  setTimePoint();
}

void Now::setNow()
{
  setTimePoint();
}

void Now::setTimePoint()
{
  if (clock_ == Clock::SYSTEM) {
    auto tp = std::chrono::system_clock::now();
    timepoint_ = tp;

  } else if (clock_ == Clock::STEADY) {
    auto tp = std::chrono::steady_clock::now();
    timepoint_ = tp;
  } else {
    auto tp = std::chrono::high_resolution_clock::now();
    timepoint_ = tp;
  }
}
