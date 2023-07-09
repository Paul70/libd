#ifndef DUTIL_FACTORYINTERFACE_H
#define DUTIL_FACTORYINTERFACE_H
#include "libd/thirdparty/typestring.h"
#include "staticpointercast.h"
#include <memory>
#include <string>

namespace DUTIL {

//! Forward declarations.
struct ConstructionData;
class Ware;

namespace FactoryInterfaceDetail {
//! Define a type trait to test if a class is a factory interface type.
template<typename T>
struct isInterface : std::false_type
{};
template<typename T>
constexpr bool isInterfaceValue = isInterface<T>::value;

//! Return the name of the concrete factory.
std::string getConcreteClassNameViaWareTypeSetting(ConstructionData const &cd);

//! Return a base class pointer (type Ware) to the concrete class built by its factory.
std::unique_ptr<Ware> newInstanceViaTypeSetting(ConstructionData const &cd, std::string interfaceName);
} // namespace FactoryInterfaceDetail

template<typename BaseType>
class FactoryInterface
{
public:
    /*! \brief Direct use of this general method is not intentioned and sensible.
     *         Use the macros below: D_DECLARE_FACTORYINTERFACE and D_DEFINE_FACTORYINTERFACE.
     *
     * This is just a fallback method if this static function is called with an invalid, i.e.
     * not registered factory interface class type.
     */
    static std::string getInterfaceName()
    {
        static_assert(FactoryInterfaceDetail::isInterfaceValue<BaseType>,
                      "The specified type parameter is not a registered interface type.");
        return "";
    }

    //! Extract the concrete class name from ConstructionData object by reading the Ware::Type parameter.
    static std::string getConcreteClassNameViaTypeSetting(ConstructionData const &cd)
    {
        return FactoryInterfaceDetail::getConcreteClassNameViaWareTypeSetting(cd);
    }

    /*! \brief Construct a new concrete object instance by evaluating the Ware::Type setting in the construction data.
     *
     * If the resulting concrete factory cannot be found, or a concrete object cannot be constructed, throw
     * an exception.
     */
    static std::unique_ptr<BaseType> newInstanceViaTypeSetting(ConstructionData const &cd)
    {
        // Base type is at least DUTIL::ProjectWare if no other custom inheritance hierarchy was established.
        return static_unique_ptr_cast<BaseType>(FactoryInterfaceDetail::getConcreteClassNameViaWareTypeSetting(cd),
                                                getInterfaceName());
    }

protected:
    //! Direct construction of FactoryInterface is not allowed.
    FactoryInterface() = default;
};

} // namespace DUTIL

/*! \brief Macros turning a class into a valid factory interface class.
 *
 * With the help of these macros it is possible to register a class as an interface
 * class for a concrete class which has a static concrete factory member.
 */
#define D_DECLARE_FACTORYINTERFACE(BASE_TYPE) \
    template<> \
    struct DUTIL::FactoryInterfaceDetail::isInterface<BASE_TYPE> : std::true_type \
    {}; \
    template<> \
    std::string DUTIL::FactoryInterface<BASE_TYPE>::getInterfaceName();

#define D_DEFINE_FACTORYINTERFACE(BASE_TYPE) \
    template<> \
    std::string DUTIL::FactoryInterface<BASE_TYPE>::getInterfaceName() \
    { \
        using Name = typestring_is(#BASE_TYPE); \
        static_assert(Name::size() > 3 && Name::data()[0] == ':' && Name::data()[1] == ':', \
                      "Interface calss name does not match libd naming rules"); \
        return Name::data(); \
    }

#endif // DUTIL_FACTORYINTERFACE_H
