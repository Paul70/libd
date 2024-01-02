#ifndef DUTIL_NAMEDREFERENCEPARAMETER_H
#define DUTIL_NAMEDREFERENCEPARAMETER_H

namespace DUTIL {

/*! \brief description of NamedReferenceParameter
 *
 * Longer description of NamedReferenceParameter.
 */
template<typename ReferredType>
class NamedReferenceParameterBase
{
public:
    //! Construct with reference parameter.
    NamedReferenceParameterBase(ReferredType r) :
        ref_(r)
    {}

    //! Return a copy of the referenced value.
    ReferredType value() const
    {
        return ref_;
    }


    ReferredType& get()
    {
        return ref_;
    }

private:
    ReferredType& ref_;
};
} // namespace DUTIL

#define D_NAMED_REFERENCE_PARAMETER(PARAMETER_NAME, REFERENCE_TPYE) \
class PARAMETER_NAME : public DUTIL::NamedReferenceParameterBase<REFERENCE_TPYE> \
{ \
public: \
    using type = REFERENCE_TPYE; \
    using DUTIL::NamedReferenceParameterBase<type>::NamedReferenceParameterBase; \
    static char const *getParameterName() \
    { \
        return #PARAMETER_NAME; \
    } \
};
#endif // DUTIL_NAMEDREFERENCEPARAMETER_H
