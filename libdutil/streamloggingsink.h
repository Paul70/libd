#ifndef DUTIL_STREAMLOGGINGSINK_H
#define DUTIL_STREAMLOGGINGSINK_H
#include "loggingsink.h"
#include <atomic>
#include <ostream>

namespace DUTIL {
struct LogItem;

/*! \brief Logging tool
 *
 * StreamLoggingSink writes log messages to the command line continously.
 */

class StreamLoggingSink : public LoggingSink
{
public:
    //! Constructor
    explicit StreamLoggingSink(std::ostream &os, LogLevel severity = LogLevel::ERROR);

    void enableCoutInAddition(bool flag = true);

private:
    virtual void acceptLogItemImpl(LogItem &&item) const override;

    std::ostream &oStream_;
    LogLevel level_;
    std::atomic<bool> stdcout_;
};

} // namespace DUTIL
#endif // DUTIL_STREAMLOGGINGSINK_H
