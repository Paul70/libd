#ifndef DUTIL_LOGGINGSINK_H
#define DUTIL_LOGGINGSINK_H
#include "namedenum.h"

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
    D_NAMED_ENUM(LogLevel, TRACE, DEBUG, INFO, WARN, ERROR, FATAL)

    //! Virtual Destructor.
    virtual ~LoggingSink() = default;

    //! Accept and handle log messages.
    //! hier muss ich noch ein time format einbauen
    void acceptLogMessage(std::string message, LogLevel level) const;

private:
    virtual void acceptLogItemImpl(LogItem &&item) const = 0;
};

} // namespace DUTIL
#endif // DUTIL_LOGGINGSINK_H
