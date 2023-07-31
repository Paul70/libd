#ifndef DUTIL_NAMEDREFERENCE_H
#define DUTIL_NAMEDREFERENCE_H
#include "variant.h"
#include <memory>
#include <string>

namespace DUTIL {
class NamedClass;

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
    using Pointer = std::shared_ptr<RT>;

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

    // fürs interface das ganze auch noch machen

    //! If the referred type is derived form DUTIL::NamedClass, this function is defined and returns the concrete class name
    //! which is also registered in the global concrete class maps (see DGLOBALS).
    template<typename ConcreteType = RT, std::enable_if_t<std::is_base_of_v<NamedClass, ConcreteType>, bool> = false>
    static std::string getReferredTypeName()
    {
        return ConcreteType::getClassName();
    }

    //! Return a copy of the reference id.
    std::string getId() const
    {
        return id_;
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
        std::string getReferenceName() const \
        { \
            return #REFERENCE_NAME; \
        } \
    };

#endif // DUTIL_NAMEDREFERENCE_H
