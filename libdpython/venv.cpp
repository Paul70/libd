#include "venv.h"

#include "libd/libdpython/pythoninterface.h"
#include "libd/libdutil/constructiondata.h"
#include "libd/libdutil/constructionvalidator.h"

using namespace DUTIL;
using namespace DPYTHON;

D_DEFINE_PROJECTWARE(Venv)

Venv::Path Venv::getCurrentWorkingDirectory()
{
  return std::filesystem::current_path();
}

ConstructionValidator const& Venv::getConstructionValidator()
{
  using SR = SettingRule;
  static const ConstructionValidator cv({[]() {
    SR sr = SR::forNamedParameter<Venv::PythonExe>(
        SR::Usage::MANDATORY_WITH_DEFAULT,
        "Set the python executable. Per default, pyhton system installation is used.");
    sr.defaultValue = "./.venv/bin/python3";
    return sr;
  }()});

  return cv;
}

Venv::Venv(DUTIL::ConstructionData const& cd, DUTIL::LoggingSource::LoggingSinkPointer sink) :
    LoggingSource(sink),
    pythonExe_(getConstructionValidator().validateNamedParameter<Venv::PythonExe>(cd))
{}

Venv::~Venv()
{
  if (isActivated())
  {
    deactivate();
  }
}

bool Venv::isActivated() const
{
  if (Py_IsInitialized())
  {
    return true;
  }
  else
  {
    return false;
  }
}

void Venv::activate() const
{
  DPYTHON::openVenvEnvironment(pythonExe_.c_str());
  DPYTHON::appendSysPaht(getCurrentWorkingDirectory().c_str());
}

void Venv::deactivate() const
{
  D_ASSERT(isActivated());
  Py_Finalize();
}

void Venv::addToSystemPath(Path const& path) const
{
  D_ASSERT(isActivated());
  DPYTHON::appendSysPaht(path.c_str());
}
