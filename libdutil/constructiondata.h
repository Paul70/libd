#ifndef DUTIL_CONSTRUCTIONDATA_H
#define DUTIL_CONSTRUCTIONDATA_H
#include "settingrule.h"
#include "settings.h"
#include <memory>

namespace DUTIL {

/*! \brief A structure for generalized construction of objects.
 *
 * This struct collects data needed to construct new objects in a flexibel an robust
 * manner. It is especially useful to have a standartizised way of handling data to object
 * constructors.
 *
 * A ConstructionData structure also provides the ability to store sub-ConstructionData
 * structures, i.e. nested ConstructionData structs.
 *
 */

struct ConstructionData
{
    //! Enum type that indicates if the ConstructionData constains real data to construct a real object or
    //! if it is just a proxy onbject needed during validation of some superior Construction data validation.
    D_NAMED_ENUM(Usage, REAL, PROXY)

    //! Settings class enables a keyword - value dictionary for basic, scalar data.
    Settings s;

    //! A std::map for recursevily storing other ConstructionData objects.
    std::map<std::string, ConstructionData> subObjectData;

    //! A std::mop containing shared pointers to objects needed for construction.
    std::map<std::string, std::shared_ptr<const Ware>> sharedWares;

    //! Forward a whole Settings object
    ConstructionData &set(Settings sNew) &;
    ConstructionData &&set(Settings sNew) &&;

    ConstructionData(Usage u);
    ConstructionData();

    bool isProxy() const;

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

    //! Helper function to register subobject data or shared wares with an unique key.
    std::string createSharedWareKeyWithCounter(std::string key) const;
    std::string createSubObjectKeyWithCounter(std::string key) const;

    //! Helper function to get back sub-ConstructionData structs  or shared wares stored in 'subObjectData' and 'sharedWares', respectively.
    std::map<std::string, ConstructionData>::const_iterator getSubObjectWithCounter(std::string key, label_t index = 0) const;

    /*! \brief Add construction data for a subobject construction.
     *
     * A ConstructionData structure holds a member 'subObjectData' which contains ConstructionData for nested subobjects.
     * Those nested subobject require own ConstructionData structures for themselves.
     *
     */
    template<typename NR>
    ConstructionData &addSubObject(ConstructionData const &cd = ConstructionData()) &
    {
        subObjectData.emplace(std::make_pair(createSubObjectKeyWithCounter(NR::getReferenceName()), cd));
        return *this;
    }

    template<typename NR>
    ConstructionData &&addSubObject(ConstructionData const &cd = ConstructionData()) &&
    {
        subObjectData.emplace(std::make_pair(createSubObjectKeyWithCounter(NR::getReferenceName()), cd));
        return std::move(*this);
    }

    /*! \brief Return a stored Sub-ConstructionData object
     *
     * This function uses the reference name and the given index to
     * identify a ConstructionData object to build a defined subobject.
     *
     * Note, ConstructionData objects needed to build subobjects are strored inside 'subObjectData' member.
     * This function does not return the overall ConstructionData structure referred to by 'this' pointer.
     *
     */
    template<typename NR>
    ConstructionData getSubObjectCd(label_t index = 0) const
    {
        auto i = getSubObjectWithCounter(NR::getReferenceName(), index);
        if (i == subObjectData.end())
            return ConstructionData();
        return i->second;
    }

    //! Define a seperator character used to differentiate a subobject key name and a count value.
    static constexpr char seperator = ';';

private:
    const Usage usage_;
};

} // namespace DUTIL

#endif // DUTIL_CONSTRUCTIONDATA_H
