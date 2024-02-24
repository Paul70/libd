#include "datasetrule.h"

using namespace DUTIL;

DatasetRule::DatasetRule() :
    usage(Usage::OPTIONAL),
    description(),
    rows(DatasetRule::lengthNotDefined),
    columns(DatasetRule::lengthNotDefined),
    type(Dataset::Type::EMPTY)
{}

DatasetRule DatasetRule::forDataset(Usage u, std::string d)
{
  DatasetRule dsr;
  dsr.usage = u;
  dsr.description = d;
  return dsr;
}
