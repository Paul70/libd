#include "datasetrule.h"

using namespace DUTIL;

DatasetRule::DatasetRule() :
    usage(Usage::OPTIONAL),
    description(),
    rows(DatasetRule::lengthNotDefined),
    columns(DatasetRule::lengthNotDefined),
    type(Dataset::Type::EMPTY)
{}

DatasetRule DatasetRule::forDataset(Usage u, std::string d, Dataset::Type t, label_t r, label_t c)
{
  DatasetRule dsr;
  dsr.usage = u;
  dsr.description = d;
  dsr.rows = r;
  dsr.columns = c;
  dsr.type = t;
  return dsr;
}
