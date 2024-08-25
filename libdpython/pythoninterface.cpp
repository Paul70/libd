#include "pythoninterface.h"
#include "libd/libdutil/exception.h"

#include <cstring>
#include <filesystem>
#include <iostream>

using namespace DUTIL;

namespace {
bool checkPySysPath(char const* scriptName)
{
  D_ASSERT(Py_IsInitialized());
  PyObject* sysPath = nullptr;
  PyObject* path = nullptr;

  sysPath = PySys_GetObject("path");
  D_ASSERT(sysPath);

  Py_ssize_t no_paths = PyList_Size(sysPath);
  for (Py_ssize_t index = 0; index < no_paths; ++index) {
    path = PyList_GetItem(sysPath, index);
    if (PyUnicode_Check(path)) {
      std::string dirPath_stdstr = PyUnicode_AsUTF8(path);
      std::filesystem::path fullPath = std::filesystem::path(dirPath_stdstr) / scriptName;

      // Check if scriptName.py exists in this directory
      if (std::filesystem::exists(fullPath)) {
        return true;
      }
    }
  }
  return false;
}
}  // namespace

namespace LIBD::DPYTHON {
void openPythonEnvironment()
{
  if (!Py_IsInitialized()) {
    Py_Initialize();
  }
}

void closePythonEnvironment()
{
  Py_Finalize();
}

void appendSysPaht(char const* path)
{
  D_ASSERT_MSG(Py_IsInitialized(), "No 'Py_Initialize()' call.");

  std::stringstream stream;
  stream << "import sys\n";
  stream << "sys.path.append('" << path << "')\n";

  PyRun_SimpleString(stream.str().c_str());
}

int runPythonScript(char const* fileName)
{
  D_ASSERT_MSG(Py_IsInitialized(), "No 'Py_Initialize()' call.");
  D_ASSERT(checkPySysPath(fileName));

  FILE* fp = ::fopen(fileName, "rb");
  if (!fp)
    D_THROW("unable to read Python script '" + std::string(fileName) + std::string("': ")
            + std::strerror(errno));

  int result = PyRun_SimpleFile(fp, fileName);
  ::fclose(fp);
  return result;
}

int runPythonModuleMethod(char const* fileName, char const* methodName,
                          std::vector<std::string> const& argv)
{
  PyObject* pModule = nullptr;
  PyObject* pName = nullptr;
  PyObject* pMethod = nullptr;
  PyObject* pArgs = nullptr;
  PyObject* pValue = nullptr;

  // PyObject pointers have to be initializes as nullptr to avoid segmentation
  // faults during debugging.
  // Do not set breakpoints before this section
  // ----------------------------------------------------------------

  D_ASSERT_MSG(Py_IsInitialized(), "No 'Py_Initialize()' call.");
  D_ASSERT(checkPySysPath(fileName));

  // import the moduel (i.e. file)
  pName = PyUnicode_DecodeFSDefault(methodName);
  pModule = PyImport_Import(pName);
  Py_DECREF(pName);

  if (pModule == nullptr) {
    D_THROW("unable to import Python method/module '" + std::string(fileName));
    return 0;
  }

  pMethod = PyObject_GetAttrString(pModule, methodName);

  if (pMethod && PyCallable_Check(pMethod)) {
    pArgs = PyTuple_New(argv.size());

    for (size_t index = 0; index < argv.size(); ++index) {
      pValue = PyUnicode_FromString(argv[index].c_str());
      if (!pValue) {
        Py_DECREF(pArgs);
        Py_DECREF(pModule);
        D_THROW("Cannot convert arguement '" + argv[index]);
        return 0;
      }

      /* pValue reference stolen here: */
      PyTuple_SetItem(pArgs, index, pValue);
    }

    pValue = PyObject_CallObject(pMethod, pArgs);
    Py_DECREF(pArgs);
    if (pValue != nullptr) {
      std::cout << "Result of call:" << PyLong_AsLong(pValue) << std::endl;
      Py_DECREF(pValue);
    } else {
      Py_DECREF(pMethod);
      Py_DECREF(pModule);
      PyErr_Print();
      fprintf(stderr, "Call failed\n");
      return 0;
    }
  }
  auto result = PyLong_AsSize_t(pValue);
  Py_XDECREF(pMethod);
  Py_DECREF(pModule);
  Py_DECREF(pValue);
  return result;
}

}  // namespace LIBD::DPYTHON
