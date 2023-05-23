#ifndef DUTIL_FACTORY_H
#define DUTIL_FACTORY_H
#include <memory>
#include <string>

namespace DUTIL {

class Factory
{
public:
    virtual ~Factory() = default;

    static bool isRegistredConcreteClass(std::string className);

protected:
    static void registerConcreteClass(std::string className, Factory* me);

    static void unregisterConcreteClass(std::string className);

private:
    std::shared_ptr<void> factoryMap_;
};

} // end namespace DUTIL
#endif // DUTIL_FACTORY_H
