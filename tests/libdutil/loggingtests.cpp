#include "libdutil/concretefactory.h"
#include "libdutil/constructiondata.h"
#include "libdutil/loggingsource.h"
#include "libdutil/namedclass.h"
#include "libdutil/projectware.h"
#include "libdutil/streamloggingsink.h"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class LoggingTests : public TestBase
{};

class Logger
{
public:
    using Sink = LoggingSource::LoggingSinkPointer;
    //! Explicit Constructor
    explicit Logger(Sink *sink = nullptr) :
        logger_(*sink)
    {}

    void writeLogMessage(std::string &&msg, LoggingSink::LogLevel severity) const
    {
        logger_.log(msg, severity);
    }

    void writeTraceLogMessage(std::string &&msg) const
    {
        logger_.trace(msg);
    }

    void writeDebugLogMessage(std::string &&msg) const
    {
        logger_.debug(msg);
    }

    void writeInfoLogMessage(std::string &&msg) const
    {
        logger_.info(msg);
    }

    void writeWarningLogMessage(std::string &&msg) const
    {
        logger_.warn(msg);
    }

    void writeErrorLogMessage(std::string &&msg) const
    {
        logger_.error(msg);
    }

    void writeFatalLogMessage(std::string &&msg) const
    {
        logger_.fatal(msg);
    }

private:
    LoggingSource logger_;
};

} // namespace

TEST_F(LoggingTests, testStreamLogging)
{
    {
        Logger::Sink pSink = std::make_shared<StreamLoggingSink>();
        Logger l(&pSink);
        l.writeLogMessage("Hello World!", LoggingSink::LogLevel::ERROR);
    }
    {
        Logger::Sink pSink = std::make_shared<StreamLoggingSink>(std::cout, LoggingSink::LogLevel::TRACE);
        Logger l(&pSink);
        l.writeTraceLogMessage("Trace log: Hello World!");
    }
    {
        Logger::Sink pSink = std::make_shared<StreamLoggingSink>(std::cout, LoggingSink::LogLevel::DEBUG);
        Logger l(&pSink);
        l.writeDebugLogMessage("Debug log: Hello World!");
    }
    {
        Logger::Sink pSink = std::make_shared<StreamLoggingSink>(std::cout, LoggingSink::LogLevel::INFO);
        Logger l(&pSink);
        l.writeInfoLogMessage("INFO log: Hello World!");
    }
    {
        Logger::Sink pSink = std::make_shared<StreamLoggingSink>(std::cout, LoggingSink::LogLevel::WARN);
        Logger l(&pSink);
        l.writeTraceLogMessage("WARNING log: Hello World!");
    }
    {
        Logger::Sink pSink = std::make_shared<StreamLoggingSink>(std::cout, LoggingSink::LogLevel::ERROR);
        Logger l(&pSink);
        l.writeErrorLogMessage("ERROR log: Hello World!");
    }
    {
        Logger::Sink pSink = std::make_shared<StreamLoggingSink>(std::cout, LoggingSink::LogLevel::FATAL);
        Logger l(&pSink);
        l.writeFatalLogMessage("FATAL log: Hello World!");
    }
}
