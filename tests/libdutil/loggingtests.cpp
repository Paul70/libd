#include "libdutil/concretefactory.h"
#include "libdutil/constructiondata.h"
#include "libdutil/loggingsource.h"
#include "libdutil/namedclass.h"
#include "libdutil/projectware.h"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class LoggingTests : public TestBase
{};

class Logger : public ProjectWare, public D_NAMED_CLASS(::Logger)
{
public:
    D_DECLARE_PROJECTWARE(Logger)

    //! Explicit Constructor
    explicit Logger(ConstructionData const &, LoggingSource::LoggingSinkPointer sink = nullptr) :
        logger_(sink)
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

D_DEFINE_PROJECTWARE(Logger)
} // namespace

TEST_F(LoggingTests, testSomethingForValueEquality)
{
    ASSERT_EQ("reference value", "actual value");
}

TEST_F(LoggingTests, testSomethingForException)
{
    //W_EXPECT_THROW("do something exceptional", "something bad is going on");
}
