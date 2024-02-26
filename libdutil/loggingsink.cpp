#include "loggingsink.h"
#include "logitem.h"

using namespace DUTIL;

void LoggingSink::acceptLogMessage(std::string message, LogSeverity severity) const
{
  LogItem li;
  li.message = message;
  li.severity = severity;
  li.clock.advance();
  acceptLogItemImpl(std::move(li));
}

void LoggingSink::setLogLevel(LogSeverity severity)
{
  setLogLevelImpl(severity);
}

void LoggingSink::readLogMessages(std::string& view, LogSeverity severity) const
{
  readLogMessagesImpl(view, severity);
}

void LoggingSink::enableCoutInAddition(bool flag)
{
  enabelCoutInAdditionImpl(flag);
}
