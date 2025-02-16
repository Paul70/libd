#ifndef LIBD_DPYTHON_VENV_H
#define LIBD_DPYTHON_VENV_H
#include "libd/libdutil/concretefactory.h"
#include "libd/libdutil/loggingsource.h"
#include "libd/libdutil/namedclass.h"
#include "libd/libdutil/projectware.h"

#include <filesystem>

namespace DUTIL {
struct ConstructionData;
}

namespace DPYTHON {

/*! \brief Class defining a .venv python environment within Slabstock for
 *         running python scripts.
 *
 * The .venv folder has to be located in the bin directory
 * of the Slabstock application. Therefore, we are able to use
 * relative paths.
 *
 * All path information is interpreted as relative paths with respect
 * to the bin directory of the executable.
 */

class Venv final :
    public DUTIL::ProjectWare,
    public DUTIL::LoggingSource,
    public D_NAMED_CLASS(::DPYTHON::Venv)
{
  public:
  using Path = std::filesystem::path;

  //! Declare static factory member
  D_DECLARE_PROJECTWARE(Venv)

  //! Parameter holding the python executable path
  D_NAMED_STRING(PythonExe);

  static Path getCurrentWorkingDirectory();

  //! Return construction requirements.
  static DUTIL::ConstructionValidator const& getConstructionValidator();

  //! Constructor using ConstructionData.
  explicit Venv(DUTIL::ConstructionData const& cd,
                DUTIL::LoggingSource::LoggingSinkPointer sink = nullptr);

  virtual ~Venv() override;

  bool isActivated() const;

  void activate() const;

  void deactivate() const;

  void addToSystemPath(Path const& path) const;

  private:
  const Path pythonExe_;

  /*! \brief Activates the venv and appends "module" path to the venv's sys
   *         path variable.
   *
   * Also appends Slabstock executable bin path as relative path to venv's
   * sys path variable.
   *
   * @param module: python module path appended to venv sys path variable.
   *
   */
  //static void openVenv(std::string_view module = "");

  /*! \brief Executes the given python method form the specified module.
   *
   * Activates a venv and executed the method in the venv context specified
   * with BIN_SYS_PATH and VENV_PATH.
   * This function opens a virtual python environment. No "openVenv()" call before
   * necessary.
   *
   * @return: PyObject pointer holding the method's return value.
   *
   */
  //static PyObject* callMethodFromModule(std::string_view method, std::string_view module);
};

}  // namespace DPYTHON
#endif  // LIBD_DPYTHON_VENV_H
