#include "factoryinterface.h"
#include "constructiondata.h"
#include "exception.h"
#include "factory.h"
#include "ware.h"

namespace DUTIL {
namespace FactoryInterfaceDetail {
std::string getConcreteClassNameViaWareTypeSetting(ConstructionData const &cd)
{
    return cd.s.getParameter<Ware::Type>();
}

std::unique_ptr<Ware> newInstanceViaTypeSetting(ConstructionData const &cd, std::string interfaceName)
{
    auto typeName = getConcreteClassNameViaWareTypeSetting(cd);
    if (!Factory::isRegisteredConcreteClassName(typeName)) {
        D_THROW("No factory found for '" + typeName + "'.");
    }
    auto const &factory = Factory::getFactoryByName(typeName);
    auto error = factory.checkCD(cd);
    if (!error.empty()) {
        D_THROW("Errors occured during ConstructionData check. Instantiating class '" + typeName + "' with interface '"
                + interfaceName + "' via factory is not possible: " + error);
    }
    return factory.newInstance(cd);
}
} // namespace FactoryInterfaceDetail
} // namespace DUTIL
