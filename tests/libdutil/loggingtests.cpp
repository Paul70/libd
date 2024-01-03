#include "libdutil/constructiondata.h"
#include "libdutil/loggingsource.h"
#include "libdutil/streamloggingsink.h"
#include "tests/testbase.h"

#include <fstream>

using namespace DUTIL;

namespace {
class LoggingTests : public TestBase
{};

class Logger
{
  public:
  using Sink = LoggingSource::LoggingSinkPointer;

  //! Explicit Constructor
  explicit Logger(Sink* sink = nullptr) :
      logger_(*sink)
  {}

  void writeLogMessage(std::string&& msg,
                       LoggingSink::LogSeverity severity) const
  {
    logger_.log(msg, severity);
  }

  void writeTraceLogMessage(std::string&& msg) const { logger_.trace(msg); }

  void writeDebugLogMessage(std::string&& msg) const { logger_.debug(msg); }

  void writeInfoLogMessage(std::string&& msg) const { logger_.info(msg); }

  void writeWarningLogMessage(std::string&& msg) const { logger_.warn(msg); }

  void writeErrorLogMessage(std::string&& msg) const { logger_.error(msg); }

  void writeFatalLogMessage(std::string&& msg) const { logger_.fatal(msg); }

  private:
  LoggingSource logger_;
};

}  // namespace

TEST_F(LoggingTests, testStreamLogging)
{
  {
    Logger::Sink pSink = std::make_shared<StreamLoggingSink>(
        ConstructionData()
            .setEnum<StreamLoggingSink::Type>(
                StreamLoggingSink::Type::OSTRINGSTREAM)
            .setEnum<LoggingSink::LogSeverity>(
                LoggingSink::LogSeverity::ERROR));
    Logger l(&pSink);
    l.writeLogMessage("Hello World!", LoggingSink::LogSeverity::ERROR);
    std::string view;
    pSink->readLogMessages(view);
    EXPECT_THAT(view, testing::HasSubstr("Hello World!"));
  }
  {
    Logger::Sink pSink = std::make_shared<StreamLoggingSink>(
        ConstructionData()
            .setEnum<StreamLoggingSink::Type>(
                StreamLoggingSink::Type::OSTRINGSTREAM)
            .setEnum<LoggingSink::LogSeverity>(
                LoggingSink::LogSeverity::TRACE));
    Logger l(&pSink);
    l.writeTraceLogMessage("Trace log: Hello World!");
    std::string view;
    pSink->readLogMessages(view);
    EXPECT_THAT(view, testing::HasSubstr("Trace log: Hello World!"));
  }
  {
    Logger::Sink pSink = std::make_shared<StreamLoggingSink>(
        ConstructionData()
            .setEnum<StreamLoggingSink::Type>(
                StreamLoggingSink::Type::OSTRINGSTREAM)
            .setEnum<LoggingSink::LogSeverity>(
                LoggingSink::LogSeverity::DEBUG));
    Logger l(&pSink);
    l.writeDebugLogMessage("Debug log: Hello World!");
    std::string view;
    pSink->readLogMessages(view);
    EXPECT_THAT(view, testing::HasSubstr("Debug log: Hello World!"));
  }
  {
    Logger::Sink pSink = std::make_shared<StreamLoggingSink>(
        ConstructionData()
            .setEnum<StreamLoggingSink::Type>(
                StreamLoggingSink::Type::OSTRINGSTREAM)
            .setEnum<LoggingSink::LogSeverity>(LoggingSink::LogSeverity::INFO));
    Logger l(&pSink);
    l.writeInfoLogMessage("INFO log: Hello World!");
    std::string view;
    pSink->readLogMessages(view);
    EXPECT_THAT(view, testing::HasSubstr("INFO log: Hello World!"));
  }
  {
    Logger::Sink pSink = std::make_shared<StreamLoggingSink>(
        ConstructionData()
            .setEnum<StreamLoggingSink::Type>(
                StreamLoggingSink::Type::OSTRINGSTREAM)
            .setEnum<LoggingSink::LogSeverity>(LoggingSink::LogSeverity::WARN));
    Logger l(&pSink);
    l.writeWarningLogMessage("WARNING log: Hello World!");
    std::string view;
    pSink->readLogMessages(view);
    EXPECT_THAT(view, testing::HasSubstr("WARNING log: Hello World!"));
  }
  {
    Logger::Sink pSink = std::make_shared<StreamLoggingSink>(
        ConstructionData()
            .setEnum<StreamLoggingSink::Type>(
                StreamLoggingSink::Type::OSTRINGSTREAM)
            .setEnum<LoggingSink::LogSeverity>(
                LoggingSink::LogSeverity::ERROR));
    Logger l(&pSink);
    l.writeErrorLogMessage("ERROR log: Hello World!");
    std::string view;
    pSink->readLogMessages(view);
    EXPECT_THAT(view, testing::HasSubstr("ERROR log: Hello World!"));
  }
  {
    Logger::Sink pSink = std::make_shared<StreamLoggingSink>(
        ConstructionData()
            .setEnum<StreamLoggingSink::Type>(
                StreamLoggingSink::Type::OSTRINGSTREAM)
            .setEnum<LoggingSink::LogSeverity>(
                LoggingSink::LogSeverity::FATAL));
    Logger l(&pSink);
    l.writeFatalLogMessage("FATAL log: Hello World!");
    std::string view;
    pSink->readLogMessages(view);
    EXPECT_THAT(view, testing::HasSubstr("FATAL log: Hello World!"));
  }
}

TEST_F(LoggingTests, testFileLogging)
{
  std::string file
      = "/home/paul/Projects/Software/libd/tests/test_data/logging/log.txt";
  {
    Logger::Sink pSink = std::make_shared<StreamLoggingSink>(
        ConstructionData()
            .setEnum<StreamLoggingSink::Type>(StreamLoggingSink::Type::FSTREAM)
            .setEnum<LoggingSink::LogSeverity>(LoggingSink::LogSeverity::ERROR)
            .setParameter<StreamLoggingSink::Path>(file));
    Logger l(&pSink);
    l.writeLogMessage("Hello World!", LoggingSink::LogSeverity::ERROR);
  }

  // read recently written log file and check content
  std::fstream test;
  test.open(file, std::ios_base::in);

  std::stringstream buffer;
  buffer << test.rdbuf();
  EXPECT_THAT(buffer.str(), testing::HasSubstr("Hello World!"));
  EXPECT_THAT(buffer.str(), testing::HasSubstr("ERROR"));
  EXPECT_THAT(buffer.str(), testing::HasSubstr("Start logging"));
}
