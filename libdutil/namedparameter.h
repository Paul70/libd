#ifndef DUTIL_NAMEDPARAMETER_H
#define DUTIL_NAMEDPARAMETER_H
#include "exception.h"
#include "variant.h"

namespace DUTIL {

/*! \brief A "smart" parameter.
 *
 * Use the macro below to associate a run-time name string with a given real or label variable.
 */
template<typename T>
class NamedParameterBase
{
public:
    //! Empty Constructor.
    NamedParameterBase() :
        value_()
    {}

    //! Constructor with value.
    NamedParameterBase(T v) :
        value_(v)
    {}

    //! Construct from DUTIL::Variant.
    NamedParameterBase(Variant const &variant) :
        value_(variant.getAs<T>().second)
    {
        if (!variant.getAs<T>().first)
            D_THROW("Variant type is not convertible into named parameter type.");
    }

    //! Return a copy of parameter value.
    T value() const
    {
        return value_;
    }

    //! Define an implicit conversion operator in order to be able to use
    //! derived class objects as their underlying parameter type, e.g. with comparision operators.
    operator T() const
    {
        return value_;
    }

private:
    T value_;
};

} // namespace DUTIL

#define D_NAMED_PARAMETER(PARAMETER_NAME, PARAMETER_TYPE) \
    class PARAMETER_NAME : public DUTIL::NamedParameterBase<PARAMETER_TYPE> \
    { \
    public: \
        using type = PARAMETER_TYPE; \
        using DUTIL::NamedParameterBase<type>::NamedParameterBase; \
        static char const *getParameterName() \
        { \
            return #PARAMETER_NAME; \
        } \
    };

#define D_NAMED_BOOL(BOOL_NAME) D_NAMED_PARAMETER(BOOL_NAME, bool);
#define D_NAMED_LABEL(LABEL_NAME) D_NAMED_PARAMETER(LABEL_NAME, DUTIL::label_t);
#define D_NAMED_REAL(REAL_NAME) D_NAMED_PARAMETER(REAL_NAME, DUTIL::real_t);

#define D_NAMED_STRING(STRING_NAME) \
    class STRING_NAME : public DUTIL::NamedParameterBase<std::string> \
    { \
    public: \
        using type = std::string; \
        using DUTIL::NamedParameterBase<type>::NamedParameterBase; \
        STRING_NAME(char const *value) : \
            DUTIL::NamedParameterBase<type>(value) \
        {} \
        [[maybe_unused]] static char const *getParameterName() \
        { \
            return #STRING_NAME; \
        } \
    };

#endif // DUTIL_NAMEDPARAMETER_H
