#include "libdutil/now.h"
#include "libdutil/constructionvalidator.h"

using namespace DUTIL;

//Now::Now() :
//    timepoint_(std::chrono::system_clock::now()),
//    tick_(0),
//    clock_(Now::Clock::SYSTEM),
//    zone_(Now::Zone::LOCAL)
//{}

//Now::Now(ConstructionData const& cd) :
//    timepoint_(),
//    tick_(getConstructionValidator().validateNamedParameter<Now::TickParam>(cd)),
//    clock_(getConstructionValidator().validateNamedEnum<Now::Clock>(cd)),
//    zone_(getConstructionValidator().validateNamedEnum<Now::Zone>(cd))
//{
//  setTimePoint();
//}

//void Now::advance(Tick ticks)
//{
//return advanceImpl(ticks);
//setTimePoint();
//return ++tick_;
//}

//void Now::setTimePoint()
//{
//  if (clock_ == Clock::SYSTEM) {
//    auto tp = std::chrono::system_clock::now();
//    timepoint_ = tp;

//  } else if (clock_ == Clock::STEADY) {
//    auto tp = std::chrono::steady_clock::now();
//    timepoint_ = tp;
//  } else {
//    auto tp = std::chrono::high_resolution_clock::now();
//    timepoint_ = tp;
//  }
//}
