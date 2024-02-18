#ifndef DUTIL_DATASETRULE_H
#define DUTIL_DATASETRULE_H
#include "dataset.h"
#include "namedenum.h"

namespace DUTIL {

/*! \brief description of DatasetRule
 *
 * Longer description of DatasetRule.
 */

struct DatasetRule
{
  static constexpr label_t lengthNotDefined = -1;

  D_NAMED_ENUM(Usage, MANDATORY, OPTIONAL)

  Usage usage;
  std::string_view description;

  label_t rows;
  label_t columns;
  Dataset::Type type;

  //! Default constructor.
  DatasetRule();

  static DatasetRule forDataset(Usage u, std::string d, Dataset::Type t = Dataset::Type::EMPTY,
                                label_t r = lengthNotDefined, label_t c = lengthNotDefined);
};

}  // namespace DUTIL
#endif  // DUTIL_DATASETRULE_H
