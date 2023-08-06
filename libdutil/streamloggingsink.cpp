#include "streamloggingsink.h"
#include "logitem.h"
#include <iostream>

namespace DUTIL {

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

void StreamLoggingSink::acceptLogItemImpl(LogItem &&item) const
{
    if (item.level <= level_) {
        oStream_ << item.message;

        if (stdcout_)
            std::cout << item.message << '\t' << item.level.toString() << ":" << '\t' << item.message << std::endl;
    }
}
} // namespace DUTIL
