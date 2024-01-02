#ifndef DUTIL_LOGGINGSINK_H
#define DUTIL_LOGGINGSINK_H
#include "namedenum.h"

#include <atomic>

namespace DUTIL {
struct LogItem;

/*! \brief LoggingSink: Inteface class that accepts log messages.
 *
 * Derived classes might write log files or print log messages vio
 * streams on the console.
 */

class LoggingSink
{
public:
    //! Define all avialable levels of log messages.
    D_NAMED_ENUM(LogSeverity, TRACE, DEBUG, INFO, WARN, ERROR, FATAL)

    //! Virtual Destructor.
    virtual ~LoggingSink() = default;

    //! Update logging level.
    void setLogLevel(LogSeverity severity);

    //! Accept and handle log messages.
    //! hier muss ich noch ein time format einbauen
    void acceptLogMessage(std::string message, LogSeverity severity) const;

    void readLogMessages(std::string& view, LogSeverity severity = LogSeverity::ERROR) const;

    //! Enable streaming of log message to std::cout additionally to another out stream.
    void enableCoutInAddition(bool flag = true);

private:
    virtual void acceptLogItemImpl(LogItem &&item) const = 0;
    virtual void enabelCoutInAdditionImpl(bool flag = true) = 0;
    virtual void setLogLevelImpl(LogSeverity severity = LogSeverity::ERROR) = 0;
    virtual void readLogMessagesImpl(std::string& view, LogSeverity severity = LogSeverity::ERROR) const = 0;
};

} // namespace DUTIL
#endif // DUTIL_LOGGINGSINK_H
