#ifndef DUTIL_SETTINGS_H
#define DUTIL_SETTINGS_H
#include "namedparameter.h"
#include "variant.h"
#include <vector>

namespace DUTIL {

/*! brief description of Settings
 *
 * Longer description of Settings.
 */

class Settings
{
public:
    //! Default-construct.
    Settings();

    template<typename NP>
    void setParameter([[maybe_unused]] NP const& v)
    {
        // setFromVariant
    }

    template<typename NP>
    NP getParameter() const
    {
        // hier rufe ich die value function mit der ich den Variant Ware::Type string auslese
        return NP();
    }

    template<typename ConcreteClass>
    void setConcreteClassParamenter() // eig kein void sondern Settings& als Rückgabewert
    {
        // setParameter<Ware::Type>()
    }

private:
    //using MapType = std::vector<std::pair<std::string, Variant>>;
    //MapType valueMap_;
};

} // namespace DUTIL

#endif // DUTIL_SETTINGS_H
