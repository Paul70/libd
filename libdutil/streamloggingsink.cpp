#include "libdutil/streamloggingsink.h"
#include "libdutil/constructionvalidator.h"
#include "libdutil/logitem.h"
#include "libdutil/settingrule.h"

#include <ostream>
#include <iostream>
#include <fstream>
#include <string_view>


namespace DUTIL {

D_DEFINE_PROJECTWARE(StreamLoggingSink);

ConstructionValidator const & StreamLoggingSink::getConstructionValidator()
{
    using SR = SettingRule;
    static ConstructionValidator cv(
        {
            [](){
                SR sr = SR::forNamedEnum<StreamLoggingSink::Type>(SR::Usage::MANDATORY_WITH_DEFAULT, "Define logging stream type.");
                sr.defaultValue = toString(StreamLoggingSink::Type::STDCOUT);
                return sr;
            }(),
            SR::forNamedParameter<StreamLoggingSink::Path>(SR::Usage::OPTIONAL, "Define log file path if needed."),
            [](){
                SR sr = SR::forNamedEnum<LoggingSink::LogSeverity>(SR::Usage::MANDATORY_WITH_DEFAULT, "Define log severity level.");
                sr.defaultValue = toString(LoggingSink::LogSeverity::ERROR);
                return sr;
            }(),
            [](){
                SR sr = SR::forNamedParameter<StreamLoggingSink::StdCout>(SR::Usage::MANDATORY_WITH_DEFAULT, "Set true for additional log output on std::cout.");
                sr.defaultValue = false;
                return sr;
            }()
        },
        {});
    return cv;
}

StreamLoggingSink::StreamLoggingSink() :
    type_(Type::STDCOUT),
    path_(""),
    stream_(std::monostate()),
    severity_(LogSeverity::ERROR),
    stdcout_(true)
{}

StreamLoggingSink::StreamLoggingSink(ConstructionData const &cd) :
    type_(getConstructionValidator().validateNamedEnum<StreamLoggingSink::Type>(cd)),
    path_(getConstructionValidator().validateNamedParameter<StreamLoggingSink::Path>(cd)),
    stream_(),
    severity_(getConstructionValidator().validateNamedEnum<LoggingSink::LogSeverity>(cd)),
    stdcout_(getConstructionValidator().validateNamedParameter<StreamLoggingSink::StdCout>(cd))
{
    using sp_fs = std::shared_ptr<std::fstream>;

    if(type_ == Type::FSTREAM)
    {
        sp_fs fs = std::make_shared<std::fstream>();

        if(fs->is_open())
            fs->close();
        fs->open(path_, std::ios_base::app);

        stream_ = fs;
        acceptLogMessage("Start logging.", severity_); // muss ich noch ändern
    }
    else if (type_ == Type::OSTRINGSTREAM)
    {
        stream_ = std::make_shared<std::ostringstream>();
    }
    else
    {
        stream_ = std::monostate();
    }
}

StreamLoggingSink::~StreamLoggingSink()
{
    if(type_ == Type::FSTREAM)
    {
        using sp_fs = std::shared_ptr<std::fstream>;
        acceptLogMessage("Stop logging, closing file stream.", severity_);
        std::get<sp_fs>(stream_)->close();
    }
}

void StreamLoggingSink::enabelCoutInAdditionImpl(bool flag)
{
    // oStream is already set to std::cout
    if(type_ == Type::STDCOUT)
        return;
    stdcout_ = flag;
}

void StreamLoggingSink::setLogLevelImpl(LogSeverity severity)
{
    severity_ = severity;
}

void StreamLoggingSink::acceptLogItemImpl(LogItem &&item) const
{
    using namespace std;
    using os = std::ostringstream;
    using fs = std::fstream;

    if (item.severity >= severity_) {
        switch (type_) {
        case Type::STDCOUT:
            break;
        case Type::FSTREAM:
            item.time.putSavedToStream<fs>(*get<shared_ptr<fs>>(stream_).get());
            *get<shared_ptr<fs>>(stream_).get() << '\t'
                                                << item.severity.toString()
                                                << " Log"
                                                <<  ":"
                                                << '\t'
                                                << item.message << std::endl;
            break;
        case Type::OSTRINGSTREAM:
            item.time.putSavedToStream<os>(*get<shared_ptr<os>>(stream_).get());
            *get<shared_ptr<os>>(stream_).get() << '\t' << item.severity.toString()
                                                << " Log"
                                                <<  ":"
                                                << '\t'
                                                << item.message << std::endl;
            break;
        default:
            break;
        }

        if (stdcout_) {
            item.time.putSavedToStream();
            std::cout << '\t' << item.severity.toString() << " Log"
                      << ":" << '\t' << item.message << std::endl;
        }
    }
}

void StreamLoggingSink::readLogMessagesImpl(std::string& view, LogSeverity) const
{
    using sp_os = std::shared_ptr<std::ostringstream>;
    if(type_ == Type::OSTRINGSTREAM)
    {
        view = std::get<sp_os>(stream_)->str();
    }
    else
    {
        D_THROW("not implemented yet");
    }
}


} // namespace DUTIL
