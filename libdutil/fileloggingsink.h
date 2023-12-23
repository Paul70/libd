#ifndef DUTIL_FILELOGGINGSINK_H
#define DUTIL_FILELOGGINGSINK_H
#include "loggingsink.h"


namespace DUTIL {
struct ConstructionData;
class ConstructionValidator;

/*! \brief description of FileLoggingSink
 *
 * Longer description of FileLoggingSink.
 */

class FileLoggingSink : public LoggingSink
{
public:
    static ConstructionValidator const & getConstructionValidator();

    // hier die gazne

    //! Construct with construction data
    explicit FileLoggingSink(ConstructionData const & cd);

private:
    virtual void acceptLogItemImpl(LogItem &&item) const override;

    std::string file_;
    std::fstream& fileStream_;
};

} // namespace DUTIL
#endif // DUTIL_FILELOGGINGSINK_H
