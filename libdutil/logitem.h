#ifndef DUTIL_LOGITEM_H
#define DUTIL_LOGITEM_H
#include "loggingsink.h"
#include "time.h"

namespace DUTIL {

/*! \brief Struct holding a log message
 * Log message contains:
 *  - the string message
 *  - a time stamp
 *  - the log severity
 */
struct LogItem
{
    Time<std::chrono::system_clock> time;
    std::string message;
    LoggingSink::LogLevel level;
};

} // namespace DUTIL
#endif // DUTIL_LOGITEM_H
