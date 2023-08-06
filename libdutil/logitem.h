#ifndef DUTIL_LOGITEM_H
#define DUTIL_LOGITEM_H
#include "loggingsink.h"
#include <chrono>

namespace DUTIL {

/*! \brief description of LogItem
 *
 * Longer description of LogItem.
 */
struct LogItem
{
    // Timestamp
    //std::chrono::
    std::string message;
    LoggingSink::LogLevel level;
};

} // namespace DUTIL
#endif // DUTIL_LOGITEM_H
