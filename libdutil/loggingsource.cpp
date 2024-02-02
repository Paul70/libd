#include "loggingsource.h"

namespace DUTIL {

LoggingSource::LoggingSource(LoggingSinkPointer sink) :
    sink_(sink)
{}

void LoggingSource::log(const std::string message, LoggingSink::LogSeverity severity) const
{
  if (sink_) {
    sink_->acceptLogMessage(message, severity);
  }
}

void LoggingSource::trace(const std::string message) const
{
  log(message, LoggingSink::LogSeverity::TRACE);
}

void LoggingSource::debug(const std::string message) const
{
  log(message, LoggingSink::LogSeverity::DEBUG);
}

void LoggingSource::info(const std::string message) const
{
  log(message, LoggingSink::LogSeverity::INFO);
}

void LoggingSource::warn(const std::string message) const
{
  log(message, LoggingSink::LogSeverity::WARN);
}

void LoggingSource::error(const std::string message) const
{
  log(message, LoggingSink::LogSeverity::ERROR);
}

void LoggingSource::fatal(const std::string message) const
{
  log(message, LoggingSink::LogSeverity::FATAL);
}

LoggingSource::LoggingSinkPointer LoggingSource::getLoggingSink() const
{
  return sink_;
}

void LoggingSource::setLoggingSink(LoggingSinkPointer sink)
{
  sink_ = sink;
}

}  // namespace DUTIL
