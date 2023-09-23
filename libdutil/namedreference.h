#ifndef DUTIL_NAMEDREFERENCE_H
#define DUTIL_NAMEDREFERENCE_H
#include "variant.h"
#include <memory>
#include <string>

namespace DUTIL {
class NamedClass;
template<class BaseType>
class FactoryInterface;
template<class T>
struct isInterface;

/*! \brief A smart reference knowing its associated id (i.e. its 'name').
 *
 * A smart reference knows its associated std::string id and has some other
 * features defined in its template base class 'NamedReferenceBase'.
 *
 * Use the macro below to define named references.
 */

template<typename ReferredType>
class NamedReferenceBase
{
public:
    using RT = ReferredType;
    using Pointer = std::shared_ptr<const RT>;

    //! Empty default constructor.
    NamedReferenceBase() = default;

    //! Construct form DUTIL::Variant and reference.
    NamedReferenceBase(Variant const &id, Pointer p = nullptr) :
        id_(id.toString()),
        ptr_(p)
    {}

    //! Construct directly from string and reference.
    NamedReferenceBase(std::string id, Pointer p = nullptr) :
        id_(id),
        ptr_(p)
    {}

    //! If the referred type is a registered factory interface, this function is defined and returns the
    //! corresponding class name.
    template<typename InterfaceType = RT, std::enable_if_t<isInterface<InterfaceType>::value, bool> = false>
    static std::string getReferredTypeName()
    {
        return FactoryInterface<InterfaceType>::getInterfaceName();
    }

    //! If the referred type is derived form DUTIL::NamedClass, this function is defined and returns the concrete class name
    //! which is also registered in the global concrete class maps (see DGLOBALS).
    template<typename ConcreteType = RT, std::enable_if_t<std::is_base_of_v<NamedClass, ConcreteType>, bool> = false>
    static std::string getReferredTypeName()
    {
        return ConcreteType::getClassName();
    }

    /*! \brief std::string conversion operator
     *
     * Returning the reference id by calling this std::string conversion operator:
     *
     *  REFERENCE_NAME r;
     *  std::string ref_id = r; // or
     *  std::string red_id = static_cast<std::string>(r);
     */
    operator std::string() const
    {
        return id_;
    }

    //! Return a copy of the reference id.
    std::string getId() const
    {
        return id_;
    }

    /*! \brief Pointer conversion operator
     *
     * Returning a REFERENCE_NAME::Pointer via conversion operator:
     *
     *  REFERENCE_NAME r;
     *  REFERENCE_NAME::Pointer p = r;
     */
    operator Pointer() const
    {
        return ptr_;
    }

    //! Return the pointer reference.
    Pointer ptr() const
    {
        return ptr_;
    }

    //! Arrow operator for pointer like usage.
    Pointer operator->() const
    {
        return ptr_;
    }

private:
    std::string id_;
    Pointer ptr_;
};
} // namespace DUTIL

#define D_NAMED_REFERENCE(REFERENCE_NAME, REFERRED_TYPE) \
    class REFERENCE_NAME : public DUTIL::NamedReferenceBase<REFERRED_TYPE> \
    { \
    public: \
        using DUTIL::NamedReferenceBase<REFERRED_TYPE>::NamedReferenceBase; \
        static const std::string getReferenceName() \
        { \
            return #REFERENCE_NAME; \
        } \
    };

#endif // DUTIL_NAMEDREFERENCE_H
