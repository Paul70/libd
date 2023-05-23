#ifndef DUTIL_FACTORYINTERFACE_H
#define DUTIL_FACTORYINTERFACE_H

namespace DUTIL {

template<typename BaseType>
class FactoryInterface
{
public:
protected:
    //! Direct construction of FactoryInterface is not allowed.
    FactoryInterface() = default;

private:
};

} // namespace DUTIL

#define D_DECLARE_FACTORYINTERFACE(BASE_TYPE)

#endif // DUTIL_FACTORYINTERFACE_H
