#include "libdutil/now.h"
#include "libdutil/constructionvalidator.h"
#include "libdutil/constructiondata.h"

using namespace DUTIL;

D_DEFINE_PROJECTWARE(Now);

ConstructionValidator const & Now::getConstructionValidator()
{
    using SR = SettingRule;
    static ConstructionValidator cv(
        {
            [](){
                SR sr = SR::forNamedEnum<Now::Clock>(SR::Usage::MANDATORY_WITH_DEFAULT, "Define clock type, default is std::chrono::system_clock.");
                sr.defaultValue = toString(Now::Clock::SYSTEM);
                return sr;
            }(),
            [](){
                SR sr = SR::forNamedEnum<Now::Zone>(SR::Usage::MANDATORY_WITH_DEFAULT, "Set time zone, default is local system time zone.");
                sr.defaultValue = toString(Now::Zone::LOCAL);
                return sr;
            }()
        },
        {}
    );
    return cv;
}

Now::Now() :
    saved_(std::chrono::system_clock::now()),
    clock_(Now::Clock::SYSTEM),
    zone_(Now::Zone::LOCAL)
{}


Now::Now(ConstructionData const &cd) :
    saved_(),
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
    if(clock_ == Clock::SYSTEM){
        auto tp = std::chrono::system_clock::now();
        saved_ = tp;

    }
    else if (clock_ == Clock::STEADY){
        auto tp = std::chrono::steady_clock::now();
        saved_ = tp;
    }
    else{
        auto tp = std::chrono::high_resolution_clock::now();
        saved_ = tp;
    }
}
