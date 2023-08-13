#include "streamloggingsink.h"
#include "logitem.h"
#include <iostream>

namespace DUTIL {

StreamLoggingSink::StreamLoggingSink() :
    oStream_(std::cout),
    level_(LogLevel::ERROR),
    stdcout_(false)
{}

StreamLoggingSink::StreamLoggingSink(std::ostream &os, LogLevel severity) :
    oStream_(os),
    level_(severity),
    stdcout_(false)
{}

void StreamLoggingSink::enableCoutInAddition(bool flag)
{
    // oStream is already set to std::cout
    if (&oStream_ == &std::cout) {
        stdcout_ = false;
    }
    stdcout_ = flag;
}

void StreamLoggingSink::setLogLevel(LogLevel level)
{
    level_ = level;
}

void StreamLoggingSink::acceptLogItemImpl(LogItem &&item) const
{
    if (item.level >= level_) {
        item.time.toStream(oStream_);
        oStream_ << '\t' << item.level.toString() << " Log"
                 << ":" << '\t' << item.message << std::endl;

        if (stdcout_) {
            item.time.toStream(std::cout);
            std::cout << '\t' << item.level.toString() << " Log"
                      << ":" << '\t' << item.message << std::endl;
        }
    }
}
} // namespace DUTIL
