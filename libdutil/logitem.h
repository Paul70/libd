#ifndef DUTIL_LOGITEM_H
#define DUTIL_LOGITEM_H
#include "clock.h"
#include "loggingsink.h"

namespace DUTIL {

/*! \brief Struct holding a log message
 * Log message contains:
 *  - the string message
 *  - a time stamp
 *  - the log severity
 */
struct LogItem
{
  Clock clock;
  std::string message;
  LoggingSink::LogSeverity severity;
};

}  // namespace DUTIL
#endif  // DUTIL_LOGITEM_H
