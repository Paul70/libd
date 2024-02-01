#ifndef DUTIL_CONSTRUCTIONVALIDATOR_H
#define DUTIL_CONSTRUCTIONVALIDATOR_H
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include "constructiondata.h"
#include "namedparameter.h"
#include "settingrule.h"
#include "warelistrule.h"

namespace DUTIL {
class Ware;
template <class BaseType>
class FactoryInterface;
template <class T>
struct isInterface;

/*! \brief description of ConstructionValidator
 *
 * Longer description of ConstructionValidator.
 */

class ConstructionValidator
{
  public:
  //! Define a fucntion callback type for custom check functions examining ConstructionData objects.
  using CheckFunction
      = std::function<std::string(ConstructionValidator const&, ConstructionData const&)>;

  //! A trivial check function for (empty) default construction which does nothing.
  static std::string trivialCheck(ConstructionValidator const&, ConstructionData const&)
  {
    return "";
  }

  //! Default check function for validating ConstructionData.
  static std::string recursiveCheck(ConstructionValidator const& cv, ConstructionData const& cd);

  //! Check if there is a type setting entry. This is important for constructing new objects via factories.
  static std::string typeSettingCheck(ConstructionData const& cd);

  //! Check if the given CD object contains real data or is only used as proxy.
  static bool proxyCheck(ConstructionData const& cd);

  //! Default Constructor.
  ConstructionValidator();

  //! Construct with construction data
  explicit ConstructionValidator(std::vector<SettingRule> settingRules,
                                 std::vector<WarelistRule> warelistRules = {},
                                 ConstructionValidator baseCV = {},
                                 CheckFunction cf = recursiveCheck);

  /*! \brief Use the check function defined at construction to check the given ConstructionData object.
   *
   * Any errors are returned in string form, an empty string implies that no errors were found.
   * This function is always available since a ConstructionValidator object is meant to be static,
   * see ConcreteWare::getConstructionValidator function.
   * Main purpose ot the this function is to call the ckeck_ functor which is responsible for calling
   *  1. check_ functors of base class ConstructionValidators,
   *  2. the default 'recursiveCheck' function and
   *  3. optional custom check-fucntions defined in ConcreteWare objects.
   */
  std::string check(ConstructionData const& cd) const;

  //! Check if a setting rule associated with the given key exists.
  bool hasSettingRule(std::string const& key) const;

  //! Check if a warelist rule associated with the given key exists.
  bool hasWarelistRule(std::string const& key) const;

  //! Return SettingRule object specified by key. If no rule is found, an exception is thrown.
  SettingRule getSettingRule(std::string const& key) const;

  //! Return all setting rule keys. If there are no setting rules, the list will be empty.
  StringList getListOfSettingRuleKeys() const;

  /*! \brief Extraction fucntions.
   *
   * These functions perform a validation before they retrun values or nested ConstructionData
   * objects from the superior ConstructionData object.
   */
  template <typename NE, std::enable_if_t<std::is_enum_v<typename NE::EnumValues>, bool> = false>
  NE validateNamedEnum(ConstructionData const& cd) const
  {
    return NE(validateSettingRuleKeyAndReturnValue(cd, NE::getEnumName()));
  }

  template <typename NP>
  NP validateNamedParameter(ConstructionData const& cd) const
  {
    return NP(validateSettingRuleKeyAndReturnValue(cd, NP::getParameterName()));
  }

  template <typename NR>
  NR validateNamedReference(ConstructionData const& cd) const
  {
    return NR(validateSharedWareAndReturnId(cd, NR::getReferenceName(), NR::getReferredTypeName()));
  }

  /*! \brief Validate construction data for a shared ware pointer.
   *
   * This function ckeck if construction data offer a std::shared_ptr<NR> to initialize a shared ware,
   * i.e. a shared member pointer.
   */
  template <typename NR>
  auto validateSharedWare(ConstructionData const& cd) const
  {
    // call NR constructor to create a NR.
    return NR(validateSharedWareAndReturnId(cd, NR::getReferenceName(), NR::getReferredTypeName()),
              std::static_pointer_cast<const typename NR::RT>(validateSharedWareAndReturnPointer(
                  cd, NR::getReferenceName(), NR::getReferredTypeName())));
  }

  template <typename NR>
  auto validateSharedList(ConstructionData const& cd) const
  {
    auto sharedWarePtrVector
        = validateSharedListAndReturnValues(cd, NR::getReferredTypeName(), NR::getReferenceName());
    std::vector<NR> resultVector(sharedWarePtrVector.size());
    std::transform(sharedWarePtrVector.begin(), sharedWarePtrVector.end(), resultVector.begin(),
                   [](auto inputItem) {
                     return NR(inputItem.first,
                               std::static_pointer_cast<const typename NR::RT>(inputItem.second));
                   });
    return resultVector;
  }

  /*! \brief Dynamically create one or more subobjects form nested construction data.
   *
   * Construction data is validated before objects are created.
   * See the helper function in the private section to understand how objects are built.
   *
   * We need the reference name here, since the warelist rule is defined for the named
   * reference.
   *
   * IMPORTANT:
   * This function may return a nullptr in case in case the input ConstructionData does not
   * contain a nested ConstructionData object to build an optional subobject.
   * Then, it is the job of the user of this fucntion to deal with this nullptr correctly.
   *
   */
  template <typename NR>
  auto buildSubobject(ConstructionData const& cd) const -> std::unique_ptr<typename NR::RT>
  {
    // Note, cd refers to the overall and not the the subobject construction data, here.
    auto& subCD = validateAndReturnSubObjectCD(cd, NR::getReferenceName());
    if (proxyCheck(subCD)) {
      return nullptr;
    }
    return makeObjectHelper<typename NR::RT>(subCD);
  }

  template <typename NR>
  auto buildSubobjectList(ConstructionData const& cd) const
  {
    // hier kommt aus irgnedeinem grung ein item zu viel raus
    auto subobjectCDPtrVec = validateAndReturnSubobjectCDs(cd, NR::getReferenceName());

    if (proxyCheck(*subobjectCDPtrVec.front())) {
      return std::vector<std::unique_ptr<typename NR::RT>>();
    }
    std::vector<std::unique_ptr<typename NR::RT>> warePtrVec(subobjectCDPtrVec.size());
    std::transform(subobjectCDPtrVec.begin(), subobjectCDPtrVec.end(), warePtrVec.begin(),
                   [](ConstructionData const* inputCD) {
                     return makeObjectHelper<typename NR::RT>(*inputCD);
                   });
    return warePtrVec;
  }

  // check functions
  template <typename NE, std::enable_if_t<std::is_enum_v<typename NE::EnumValues>, bool> = false>
  std::string checkNamedEnum(ConstructionData const& cd) const
  {
    return checkSettingRuleKeyAndReturnErrors(cd, NE::getEnumName());
  }

  template <typename NP>
  std::string checkNamedParameter(ConstructionData const& cd) const
  {
    return checkSettingRuleKeyAndReturnErrors(cd, NP::getParameterName());
  }

  template <typename NR>
  std::string checkSubobject(ConstructionData const& cd) const
  {
    return checkSubObjectAndReturnErrors(cd, NR::getReferenceName());
  }

  template <typename NR>
  std::string ckeckSubobjectList(ConstructionData const& cd) const
  {
    return checkSubObjectListAndReturnErrors(cd, NR::getReferenceName());
  }

  template <typename NR>
  std::string checkSharedWare(ConstructionData const& cd) const
  {
    return checkSharedWareAndReturnErrors(cd, NR::getReferenceName(), NR::getReferredTypeName());
  }

  template <typename NR>
  std::string checkSharedWareList(ConstructionData const& cd) const
  {
    return checkSharedWareListAndReturnErrors(cd, NR::getReferenceName(),
                                              NR::getReferredTypeName());
  }

  private:
  /*! \brief Check function returning settings value.
   *
   * Checks the value if it meets all its associated setting rules and in case of success returns the value as
   * a DUTIL::Variant.
   * If a setting rule check fails, an MONOSTATE variant object will be returned.
   */
  Variant checkSettingRuleKeyAndReturnValue(Variant const value, std::string const& key,
                                            std::string& error) const;

  /*! \brief Check functions.
   *
   * This set of functions are calles from their validate... counterpart functions and
   * take a registered setting or warelist rule to test if the construction data parameter or construction data subobject
   * fullfills that rule.
   */
  std::string checkSettingRuleKeyAndReturnErrors(ConstructionData const& cd,
                                                 std::string const& key) const;
  std::string checkSubObjectAndReturnErrors(ConstructionData const& cd,
                                            std::string const& key) const;
  std::string checkSubObjectListAndReturnErrors(ConstructionData const& cd,
                                                std::string const& key) const;
  std::string checkSharedWareAndReturnErrors(ConstructionData const& cd, std::string const& key,
                                             std::string const& referredTypeName) const;
  std::string checkSharedWareListAndReturnErrors(ConstructionData const& cd, std::string const& key,
                                                 std::string const& referredTypeName) const;

  /*! \brief Validation functions.
   *
   * This set of functions checks
   * - if s setting rule or warelist rule is available for the construction data parameter associated with the gieven key,
   * - calls the corresponding ckeck... function to test if the parameter fulfills the setting or warelist rule and
   * - crates a DUTIL::Variant containing the parameter value extracted from construction data structure or a whole
   *   ConstructionData struct itself to build subobjects.
   */
  Variant validateSettingRuleKeyAndReturnValue(ConstructionData const& cd,
                                               std::string const key) const;
  ConstructionData const& validateAndReturnSubObjectCD(ConstructionData const& cd,
                                                       std::string const key) const;
  std::vector<ConstructionData const*> validateAndReturnSubobjectCDs(ConstructionData const& cd,
                                                                     std::string const& key) const;
  std::string validateSharedWareAndReturnId(ConstructionData const& cd, std::string const& key,
                                            std::string const& referredTypeName) const;
  std::shared_ptr<const Ware> validateSharedWareAndReturnPointer(
      ConstructionData const& cd, std::string const& key,
      std::string const& referredTypeName) const;
  std::vector<std::pair<std::string, std::shared_ptr<const Ware>>>
  validateSharedListAndReturnValues(ConstructionData const& cd, std::string const& key,
                                    std::string const& referredTypeName) const;

  /*! \brief Helper functions.
   *
   * These functions help to call factories to create one or more objects during runtime.
   */
  template <typename T,
            std::enable_if_t<std::is_constructible_v<T, ConstructionData>, bool> = false,
            std::enable_if_t<!isInterface<T>::value, bool> = false>
  static std::unique_ptr<T> makeObjectHelper(ConstructionData const& cd)
  {
    return std::make_unique<T>(cd);
  };

  template <typename WareInterface,
            std::enable_if_t<isInterface<WareInterface>::value, bool> = false>
  static std::unique_ptr<WareInterface> makeObjectHelper(ConstructionData const& cd)
  {
    auto error = typeSettingCheck(cd);
    if (!error.empty())
      D_THROW(error);
    return FactoryInterface<WareInterface>::newInstanceViaTypeSetting(cd);
  };

  //! Members
  std::map<std::string, SettingRule> settingRules_;
  std::map<std::string, WarelistRule> warelistRules_;
  CheckFunction check_;
};

}  // namespace DUTIL
#endif  // DUTIL_CONSTRUCTIONVALIDATOR_H
