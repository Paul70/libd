#include "loggingsink.h"
#include "logitem.h"

using namespace DUTIL;

void LoggingSink::acceptLogMessage(std::string message, LogLevel level) const
{
    LogItem li;
    li.message = message;
    li.level = level;
    // hier muss noch der timestamp rein
    acceptLogItemImpl(std::move(li));
}
