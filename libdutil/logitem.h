#ifndef DUTIL_LOGITEM_H
#define DUTIL_LOGITEM_H
#include "libdutil/loggingsink.h"
#include "libdutil/now.h"

namespace DUTIL {

/*! \brief Struct holding a log message
 * Log message contains:
 *  - the string message
 *  - a time stamp
 *  - the log severity
 */
struct LogItem
{
    //Now<std::chrono::system_clock> time;
    Now time;
    std::string message;
    LoggingSink::LogSeverity severity;
};

} // namespace DUTIL
#endif // DUTIL_LOGITEM_H
