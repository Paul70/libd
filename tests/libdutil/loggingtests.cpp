#include "libdutil/concretefactory.h"
#include "libdutil/constructiondata.h"
#include "libdutil/loggingsource.h"
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
        std::ostringstream logStream;
        Logger::Sink pSink = std::make_shared<StreamLoggingSink>(logStream);
        Logger l(&pSink);
        l.writeLogMessage("Hello World!", LoggingSink::LogLevel::ERROR);
        EXPECT_THAT(logStream.str(), testing::HasSubstr("Hello World!"));
    }
    {
        std::ostringstream logStream;
        Logger::Sink pSink = std::make_shared<StreamLoggingSink>(logStream, LoggingSink::LogLevel::TRACE);
        Logger l(&pSink);
        l.writeTraceLogMessage("Trace log: Hello World!");
        EXPECT_THAT(logStream.str(), testing::HasSubstr("Trace log: Hello World!"));
    }
    {
        std::ostringstream logStream;
        Logger::Sink pSink = std::make_shared<StreamLoggingSink>(logStream, LoggingSink::LogLevel::DEBUG);
        Logger l(&pSink);
        l.writeDebugLogMessage("Debug log: Hello World!");
        EXPECT_THAT(logStream.str(), testing::HasSubstr("Debug log: Hello World!"));
    }
    {
        std::ostringstream logStream;
        Logger::Sink pSink = std::make_shared<StreamLoggingSink>(logStream, LoggingSink::LogLevel::INFO);
        Logger l(&pSink);
        l.writeInfoLogMessage("INFO log: Hello World!");
        EXPECT_THAT(logStream.str(), testing::HasSubstr("INFO log: Hello World!"));
    }
    {
        std::ostringstream logStream;
        Logger::Sink pSink = std::make_shared<StreamLoggingSink>(logStream, LoggingSink::LogLevel::WARN);
        Logger l(&pSink);
        l.writeWarningLogMessage("WARNING log: Hello World!");
        EXPECT_THAT(logStream.str(), testing::HasSubstr("WARNING log: Hello World!"));
    }
    {
        std::ostringstream logStream;
        Logger::Sink pSink = std::make_shared<StreamLoggingSink>(logStream, LoggingSink::LogLevel::ERROR);
        Logger l(&pSink);
        l.writeErrorLogMessage("ERROR log: Hello World!");
        EXPECT_THAT(logStream.str(), testing::HasSubstr("ERROR log: Hello World!"));
    }
    {
        std::ostringstream logStream;
        Logger::Sink pSink = std::make_shared<StreamLoggingSink>(logStream, LoggingSink::LogLevel::FATAL);
        Logger l(&pSink);
        l.writeFatalLogMessage("FATAL log: Hello World!");
        EXPECT_THAT(logStream.str(), testing::HasSubstr("FATAL log: Hello World!"));
    }
}
