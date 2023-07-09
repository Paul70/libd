#ifndef DUTIL_SETTINGS_H
#define DUTIL_SETTINGS_H
#include "basictypes.h"
#include "variant.h"
#include "ware.h"
#include <vector>

namespace DUTIL {

/*! \brief A key - value container class for defining and storing parameters.
 *
 * This class basically defines a data container mapping a string names with
 * its defined value. It allows you to extract data by providing the string key.
 *
 */

class Settings
{
public:
    //! Default-construct an empty Settings object.
    Settings();

    //! Check if the Settings object contains any key-value pairs.
    bool empty() const;

    //! Check if Setings object already has this registered key.
    bool hasKey(std::string const &key) const;

    //! Return a list containing all currently registered keys. List can be empty.
    StringList keys() const noexcept;

    /*! \brief Retrun a copy of the value specified by the given key.
     *
     * If no value for the given key is found, the default value will be returned.
     * In case of an empty key value the default value will be returned.
     * This function does not alter the object.
     */
    Variant value(std::string const key, Variant const &defaultValue = Variant()) const;

    /*! \brief Emplaced back a new key-value pair to the map.
     *
     * If the given key already exists, this key entry gets overriden with
     * the new value and the map size stays the same.
     * In case of an empty key, nothing is added and the function just returns.
     */
    Settings &setFromVariant(std::string key, DUTIL::Variant value);

    /*! \brief Remove a value specified by the given key.
     *
     * If no value for the given key is found or in case of an empty key, nothing happens.
     */
    Settings &erase(std::string const key);

    /*! \brief Add a new settings key-value pair.
     *
     * This function is a shortcut method for "setFromVariant". Already existing keys
     * will be overriden with the new value.
     */
    template<typename ConvertibleToVariant>
    Settings &set(std::string key, ConvertibleToVariant value)
    {
        return setFromVariant(key, DUTIL::Variant(value));
    }

    /*! \brief Shortcut mehtod for adding a NamedEnum to the key-value map.
     *
     * The named enum class name serves as key value.
     *
     * Usage example:
     *
     * - from a NamedEnum object (NE):
     *   WEEKDAY day {WEEKDAY::Monday};
     *   Settings s = Settings().setEnum(day);
     * - from a NamedEnum value (NEV):
     *   Settings s = Settings().setEnum(WEEKDAY::Monday);
     *
     * Both calls result in the following key-value entry:
     * key: std::string("WEEKDAY"), value: Variant(std::string("Monday"))
     */
    template<typename NEV, std::enable_if_t<std::is_enum_v<NEV>, bool> = false>
    Settings &setEnum(NEV const &nev)
    {
        auto ne = fromEnumValue(nev);
        return set(ne.getEnumName(), ne);
    }
    template<typename NE, std::enable_if_t<std::is_enum_v<typename NE::EnumValues>, bool> = false>
    Settings &setEnum(NE const &ne)
    {
        return set(ne.getEnumName(), ne);
    }

    /*! \brief Shortcut mehtod for extracting a NamedEnum object.
     *
     * This function basically calls Settings::value function.
     * Assumed the enum has been stored using the Settings::setEnum function
     * which actually stores its string representation.
     */
    template<typename NE, std::enable_if_t<std::is_enum_v<typename NE::EnumValues>, bool> = false>
    NE getEnum() const
    {
        return NE(value(NE::getEnumName()));
    }

    /*! \brief Shortcut mehtod for storing a NamedParameter object.
     *
     * The NamedParameter object is used to construct a Variant object.
     * Afterwards, the Settings::setFromVariant is used to store the value.
     * The NamedParameter std::string name serves as map key.
     *
     * Usage example:
     *  D_NAMED_STRING(StringWithName)
     *  StringWithName swn;
     *  Settings s = Settings().setParameter(swn);
     *
     *  Created key-value pair:
     *  Key: std::string("StringWithName"), Value: DUTIL::Variant(StringWithName.value())
     */
    template<typename NP>
    Settings &setParameter(NP const &v)
    {
        return setFromVariant(NP::getParameterName(), Variant(v.value()));
    }

    /*! \brief Shortcut mehtod for extracting a NamedParameter objects.
     *
     * This function basically calls Settings::value function.
     * Assumed the named parameter has been stored using the Settings::setParameter function
     * which actually stores its string representation.
     */
    template<typename NP>
    NP getParameter() const
    {
        return NP(value(NP::getParameterName()));
    }

    /*! \brief Shortcut mehtod for storing ConcreteClass objects.
     *
     * A ConcreteClass is stored as follows:
     *  Key: Value of NamedString Type which each ConcreteClass object owns.
     *  Value: Variant holding the ConcreteClassName itsself set by D_NAMED_CLASS macro.
     *
     * Usage:
     * Settings s = Settings().setConcreteClass<CONCRETE_CLASS>();
     */
    template<typename ConcreteClass>
    Settings &setConcreteClassParamenter()
    {
        return setParameter<Ware::Type>(ConcreteClass::getClassName());
    }

    /*! \brief Lexicographical comparison operator.
     *
     * Implementation uses the std::tie pattern for operators.
     * Operators are friend functions to be able to use lhs and rhs arguments.
     */
    friend bool operator==(Settings const &lhs, Settings const &rhs);
    friend bool operator!=(Settings const &lhs, Settings const &rhs);

protected:
    //! Declare the map type. A vector is used because we want to have
    //! a sequential container.
    using MapType = std::vector<std::pair<std::string, DUTIL::Variant>>;

    //! Return a reference to the map member.
    MapType const &get() const;

private:
    MapType valueMap_;
};

} // namespace DUTIL

#endif // DUTIL_SETTINGS_H
