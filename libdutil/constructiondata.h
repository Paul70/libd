#ifndef DUTIL_CONSTRUCTIONDATA_H
#define DUTIL_CONSTRUCTIONDATA_H
#include "settings.h"

namespace DUTIL {

/*! brief description of ConstructionData
 *
 * Longer description of ConstructionData.
 *
 * https://en.cppreference.com/w/cpp/language/member_functions#ref-qualified_member_functions
 *
 */

struct ConstructionData
{
    Settings s;

    //! Forward a whole Settings object
    ConstructionData &set(Settings sNew) &;
    ConstructionData &&set(Settings sNew) &&;

    //! Forward a NamedEnum object. Enum object is stored in Settings member.
    template<typename NE>
    ConstructionData &setEnum(NE const &ne) &
    {
        s.setEnum(ne);
        return *this;
    }

    template<typename NE>
    ConstructionData &&setEnum(NE const &ne) &&
    {
        s.setEnum(ne);
        return std::move(*this);
    }

    //! Forward a NamedParmeter object. Parameter object is stored in Settings member.
    template<typename NP>
    ConstructionData &setParamter(NP const &np) &
    {
        s.setParameter(np);
        return *this;
    }

    template<typename NP>
    ConstructionData &&setParamter(NP const &np) &&
    {
        s.setParameter(np);
        return std::move(*this);
    }

    //! Forward a ConcreteClass object into Settings member.
    template<typename ConcreteClass>
    ConstructionData &setConcreteClassParameter() &
    {
        s.setConcreteClassParameter<ConcreteClass>();
        return *this;
    }

    template<typename ConcreteClass>
    ConstructionData &&setConcreteClassParameter() &&
    {
        s.setConcreteClassParameter<ConcreteClass>();
        return std::move(*this);
    }
};

} // namespace DUTIL

#endif // DUTIL_CONSTRUCTIONDATA_H
