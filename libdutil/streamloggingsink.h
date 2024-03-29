#ifndef DUTIL_STREAMLOGGINGSINK_H
#define DUTIL_STREAMLOGGINGSINK_H

#include "concretefactory.h"
#include "loggingsink.h"
#include "namedclass.h"
#include "namedparameter.h"
#include "projectware.h"

#include <atomic>
#include <filesystem>
#include <variant>

namespace DUTIL {
class ConstructionValidator;
struct LogItem;
struct ConstructionData;

/*! \brief Logging tool
 *
 * StreamLoggingSink writes log messages to the command line continously.
 */
class StreamLoggingSink final :
    public LoggingSink,
    public ProjectWare,
    public D_NAMED_CLASS(::DUTIL::StreamLoggingSink)
{
  public:
  D_DECLARE_PROJECTWARE(StreamLoggingSink)

  using StreamType
      = std::variant<std::shared_ptr<std::fstream>, std::shared_ptr<std::ostream>, std::monostate>;

  static ConstructionValidator const& getConstructionValidator();

  //! Define allowed stream types. OSTREAM case will refer to a std::stringstream.
  D_NAMED_ENUM(Type, STDCOUT, FSTREAM, OSTREAM)

  //! Variable holding log file path.
  D_NAMED_STRING(Path)

  //! Flag, indicating if std::cout is used.
  D_NAMED_BOOL(StdCout)

  //! Default constructor, only std::cout for logging.
  StreamLoggingSink();

  //! Constructor using ConstructionData
  explicit StreamLoggingSink(ConstructionData const& cd);

  //! Construct by providing a string-stream object.
  explicit StreamLoggingSink(std::shared_ptr<std::ostream> os,
                             LoggingSink::LogSeverity level = LoggingSink::LogSeverity::ERROR);

  virtual ~StreamLoggingSink() override;

  void getStream(StreamType& var);

  private:
  virtual void acceptLogItemImpl(LogItem&& item) const override;
  virtual void enabelCoutInAdditionImpl(bool flag = true) override;
  virtual void setLogLevelImpl(LogSeverity severity) override;
  virtual void readLogMessagesImpl(std::string& view,
                                   LogSeverity severity = LogSeverity::ERROR) const override;

  const Type type_;
  const std::filesystem::path path_;

  // std::variant holding a shared pointer
  StreamType stream_;
  LogSeverity severity_;
  std::atomic<bool> stdcout_;
};

}  // namespace DUTIL
#endif  // DUTIL_STREAMLOGGINGSINK_H
