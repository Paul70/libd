#ifndef DUTIL_WARE_H
#define DUTIL_WARE_H
#include <string>
#include "namedparameter.h"

namespace DUTIL {

/*! \brief Interface class for objects that can be build using a concrete factroy.
 *
 * This class is needed since we have to set a starting point. The main purpose
 * is to provide a least significant basis for all derived classes constructable
 * via factories. This base class contains the concrete class name and nothing more.
 */
class Ware
{
  public:
  //! NamedStirng member used as a key to identify concrete classes.
  D_NAMED_STRING(DUTIL_Ware_Type)

  //! Function for getting the full concrete class name.
  std::string getConcreteClassName() const;

  //! Function for getting the short (i.e. without namespace names) concrete class name.
  std::string getShortConcreteClassName() const;

  virtual ~Ware() = default;
};

}  // namespace DUTIL

#endif  // DUTIL_WARE_H
