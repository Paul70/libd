#ifndef DUTIL_NAMEDPARAMETER_H
#define DUTIL_NAMEDPARAMETER_H

namespace DUTIL {

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

    //! Return a copy of parameter value.
    T value() const
    {
        return value_;
    }

    //! Bracket operator to return a copy of parameter value.
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
        [[maybe_unused]] static char const* getParameterName() \
        { \
            return #PARAMETER_NAME; \
        } \
    };

#define D_NAMED_BOOL(BOOL_NAME) D_NAMED_PARAMETER(BOOL_NAME, bool)

#define D_NAMED_STRING(STRING_NAME) \
    class STRING_NAME : public DUTIL::NamedParameterBase<std::string> \
    { \
    public: \
        using type = std::string; \
        STRING_NAME(std::string const v) : DUTIL::NamedParameterBase<std::string>(std::string(v)) \
        {} \
        STRING_NAME(char const* v) : DUTIL::NamedParameterBase<std::string>(std::string(v)) \
        {} \
        [[maybe_unused]] static char const* getParameterName() \
        { \
            return #STRING_NAME; \
        } \
    };

#endif // DUTIL_NAMEDPARAMETER_H
