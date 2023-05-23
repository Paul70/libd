#ifndef DUTIL_NAMEDCLASS_H
#define DUTIL_NAMEDCLASS_H
#include "libd/thirdparty/typestring.h"
#include <string>

namespace DUTIL {

class NamedClass
{
public:
    virtual ~NamedClass() = default;

    std::string getClassName() const
    {
        return getClassNameImpl();
    }

    std::string getShortClassName() const
    {
        return getShortClassNameImpl();
    }

protected:
    static std::string shortenName(std::string const input)
    {
        auto pos = (input.find_last_of(':'));
        if(pos != std::string::npos)
            return input.substr(++pos, input.size());
        else
            return input;
    }

private:
    virtual std::string getClassNameImpl() const = 0;
    virtual std::string getShortClassNameImpl() const = 0;
};

template<typename Name>
class NamedClassHelper : public NamedClass
{
public:
    static std::string getClassName()
    {
        return Name::data();
    }

    static std::string getShortClassName()
    {
        return NamedClass::shortenName(Name::data());
    }

private:
    static_assert(*Name::data() == ':' && *(Name::data() + 1) == ':' && Name::size() > 3, "Class name identifier must follow the pattern \"::Organisation::Project::ClassName\" ");

    virtual std::string getClassNameImpl() const override final
    {
        return NamedClassHelper::getClassName();
    }

    virtual std::string getShortClassNameImpl() const override final
    {
        return NamedClassHelper::getShortClassName();
    }
};

} // namespace DUTIL

/*!
 * \brief Explanation for makro D_NAMED_CLASS(TYPE)
 *
 * The typestring.h header lib is used to define the template parameter arguement for NamedClassHelper
 * at compile time. In ohter words, a string makro input is transformed into a C++ type at during compilation.
 * This let's us access the newly created type inside NamedClassHelper via the template parameter "Name".
 */
#define D_NAMED_CLASS(TYPE) DUTIL::NamedClassHelper<typestring_is(#TYPE)>
#endif // DUTIL_NAMEDCLASS_H
