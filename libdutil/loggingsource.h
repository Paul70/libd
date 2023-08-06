#ifndef DUTIL_LOGGINGSOURCE_H
#define DUTIL_LOGGINGSOURCE_H
#include "loggingsink.h"
#include <memory>

namespace DUTIL {

/*! \brief Mixin class to write log messages.
 *
 * Connect this class with a concete logging sink, e.g. StreamLoggingSink to
 * write log messages. Set the logging verbosity via the different log levels
 * porvided by the logging sink.
 */

class LoggingSource
{
public:
    using LoggingSinkPointer = std::shared_ptr<LoggingSink>;

    //! Declaration of explicit constructor,
    explicit LoggingSource(LoggingSinkPointer sink = nullptr);

    //! Write a log message into the logging sink.
    void log(const std::string message, LoggingSink::LogLevel severity = LoggingSink::LogLevel::INFO) const;

    //! Shortcut logging fucntions for different log severity, i.e. using higher log levels.
    void trace(const std::string message) const;
    void debug(const std::string message) const;
    void info(const std::string message) const;
    void warn(const std::string message) const;
    void error(const std::string message) const;
    void fatal(const std::string message) const;

    //! Get a shared pointer to the underlying logging sink.
    LoggingSinkPointer getLoggingSink() const;

    //! Set the logging sink
    void setLoggingSink(LoggingSinkPointer sink);

private:
    LoggingSinkPointer sink_;
};

} // namespace DUTIL
#endif // DUTIL_LOGGINGSOURCE_H
