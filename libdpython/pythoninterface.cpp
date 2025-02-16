#include "pythoninterface.h"
#include "libd/libdutil/exception.h"
#include "libd/libdutil/settings.h"

#include <cstring>
#include <cwchar>
#include <filesystem>
#include <iostream>
#include <optional>

using namespace DUTIL;

namespace {

std::optional<std::wstring> convertToWString(std::string_view input)
{
  // get length of resulting wstring
  auto state = std::mbstate_t();
  const char* src = input.data();
  auto length = std::mbsrtowcs(nullptr, &src, 0, &state);

  // transfrom into wstring
  if (length != static_cast<size_t>(-1))
  {
    std::wstring wstr{};
    wstr.resize(length);
    length = std::mbsrtowcs(wstr.data(), &src, length, &state);

    // check for conversion error
    if (length == static_cast<size_t>(-1) || errno == EILSEQ)
    {
      return {};
    }
    return {wstr};
  }

  return {};
}

bool checkPySysPath(std::string const& file)
{
  PyObject* sysPath = nullptr;
  PyObject* path = nullptr;

  // PyObject pointers have to be initializes as nullptr to avoid segmentation
  // faults during debugging. Do not set breakpoints before this section
  // --------------------------------------------------------------------------
  D_ASSERT(Py_IsInitialized());

  sysPath = PySys_GetObject("path");
  D_ASSERT(sysPath);

  Py_ssize_t noPathItems = PyList_Size(sysPath);
  for (Py_ssize_t index = 0; index < noPathItems; ++index)
  {
    path = PyList_GetItem(sysPath, index);
    if (PyUnicode_Check(path))
    {
      std::string dirPath_stdstr = PyUnicode_AsUTF8(path);
      std::filesystem::path fullPath = std::filesystem::path(dirPath_stdstr) / file.data();

      // Check pyFile exists in this directory
      if (std::filesystem::exists(fullPath))
      {
        return true;
      }
    }
  }
  return false;
}

std::string fetchPyError()
{
  PyObject *exc_type = nullptr, *exc_value = nullptr, *exc_traceback = nullptr,
           *exc_value_str = nullptr;

  // PyObject pointers have to be initializes as nullptr to avoid segmentation
  // faults during debugging. Do not set breakpoints before this section
  // --------------------------------------------------------------------------

  // Fetch the error information
  PyErr_Fetch(&exc_type, &exc_value, &exc_traceback);
  PyErr_NormalizeException(&exc_type, &exc_value, &exc_traceback);

  // Convert the error value to a string
  PyObject_Str(exc_value);
  std::string error{PyUnicode_AsUTF8(exc_value_str)};

  Py_XDECREF(exc_type);
  Py_XDECREF(exc_value);
  Py_XDECREF(exc_traceback);
  Py_XDECREF(exc_value_str);

  return error;
}
}  // namespace

namespace DPYTHON {

void openSystemEnvironment()
{
  if (Py_IsInitialized())
  {
    return;
  }

  Py_Initialize();
  if (!Py_IsInitialized())
  {
    D_THROW("Failed to initialize Python system runtime evironment");
  }
}

void openVenvEnvironment(std::string_view venvPaht)
{
  auto wstr_venvPath = convertToWString(venvPaht);
  if (Py_IsInitialized())
  {
    std::wstring installation{Py_GetExecPrefix()};
    if (installation.find(wstr_venvPath.value()))
    {
      return;
    }
  }

  PyConfig config;
  PyConfig_InitIsolatedConfig(&config);

  try
  {
    PyConfig_SetString(&config, &config.executable, wstr_venvPath->data());
  }
  catch (std::bad_optional_access const& e)
  {
    D_THROW("Failed to open Python Venv." + e.what());
    return;
  }
  catch (...)
  {
    D_THROW("Failed to open Python Venv. Unkown error");
    return;
  }

  auto status = Py_InitializeFromConfig(&config);
  PyConfig_Clear(&config);

  if (PyStatus_Exception(status))
  {
    D_THROW(status.err_msg);
  }
}

void closePythonEnvironment() noexcept
{
  Py_Finalize();
}

void appendSysPaht(std::string_view path)
{
  D_ASSERT(Py_IsInitialized());

  std::stringstream stream;
  stream << "import sys\n";
  stream << "sys.path.append('" << path << "')\n";
  stream << "print(sys.path)";

  PyRun_SimpleString(stream.str().c_str());
}

std::string runPythonCode(std::filesystem::path file, std::string_view method, std::string dto,
                          std::shared_ptr<std::string> outputCli)
{
  PyObject* pModule = nullptr;
  PyObject* pFunc = nullptr;
  PyObject* pArgs = nullptr;
  PyObject* pResult = nullptr;

  PyObject* sys = nullptr;
  PyObject* io = nullptr;
  PyObject* string_io = nullptr;

  // PyObject pointers have to be initializes as nullptr to avoid segmentation
  // faults during debugging. Do not set breakpoints before this section
  // --------------------------------------------------------------------------

  // Check if python runtime environment is initialized and
  // if the xxx.py file path exists in SYSPATH env variable.
  D_ASSERT(Py_IsInitialized());
  D_ASSERT(file.extension() == ".py");
  D_ASSERT(checkPySysPath(file.filename()));

  // for std_cout and std_err redirection
  if (outputCli)
  {
    sys = PyImport_ImportModule("sys");
    io = PyImport_ImportModule("io");

    // Create a new StringIO object
    string_io = PyObject_CallMethod(io, "StringIO", nullptr);
    if (string_io == nullptr)
    {
      Py_XDECREF(io);
      Py_XDECREF(sys);
      D_THROW("Python error redirecting stderr/stdout:" + fetchPyError());
      return {};
    }
    // Redirect sys.stdout and sys.stderr to StringIO
    PyObject_SetAttrString(sys, "stdout", string_io);
    PyObject_SetAttrString(sys, "stderr", string_io);
  }

  // Import the Python module method.
  //auto re = file.stem().c_str();
  pModule = PyImport_ImportModule(std::string(file.stem()).data());

  if (pModule == nullptr)
  {
    //PyErr_Print();
    if (outputCli)
    {
      Py_XDECREF(io);
      Py_XDECREF(sys);
      Py_XDECREF(string_io);
    }
    D_THROW("Python module/file error:" + fetchPyError());
  }

  pFunc = PyObject_GetAttrString(pModule, method.data());
  if (pFunc == nullptr)
  {
    //PyErr_Print();
    if (outputCli)
    {
      Py_XDECREF(io);
      Py_XDECREF(sys);
      Py_XDECREF(string_io);
    }
    D_THROW("Python method error:" + fetchPyError());
  }

  // 3. Step: Execute Python code.
  if (PyCallable_Check(pFunc))
  {
    pArgs = PyTuple_Pack(1, PyUnicode_FromString(std::move(dto.c_str())));
    if (!pArgs)
    {
      if (outputCli)
      {
        Py_XDECREF(io);
        Py_XDECREF(sys);
        Py_XDECREF(string_io);
      }
      D_THROW("Python args error:" + fetchPyError());
    }

    // call python method, pArgs is allowed to be nil.
    pResult = PyObject_CallObject(pFunc, pArgs);

    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
    Py_DECREF(pModule);

    if (pResult == nullptr)
    {
      //PyErr_Print();
      if (outputCli)
      {
        Py_XDECREF(io);
        Py_XDECREF(sys);
        Py_XDECREF(string_io);
      }
      D_THROW("Executing Python code failed:" + fetchPyError());
    }
    else
    {
      if (outputCli)
      {
        outputCli->append(PyUnicode_AsUTF8(PyObject_CallMethod(string_io, "getvalue", nullptr)));
        Py_XDECREF(io);
        Py_XDECREF(sys);
        Py_XDECREF(string_io);
      }

      if (PyBytes_Check(pResult))
      {

        char* bytes = PyBytes_AsString(pResult);
        return std::string(bytes);
      }
      // if (PyBool_Check(pResult))
      // {
      //   // hier bin ich drin
      //   return {};
      // }
      if (PyUnicode_Check(pResult))
      {
        auto result = PyUnicode_AsUTF8(pResult);
        return result;
      }
      return {};

      //auto result = PyUnicode_AsUTF8(pResult);
      //Py_XDECREF(pResult);
      //return result;
    }
  }
  else
  {
    D_THROW("Py error: Function is not callable:" + fetchPyError());
  }

  // Get the output from the StringIO object
  //output = PyObject_CallMethod(string_io, "getvalue", nullptr);
  //std::string result = PyUnicode_AsUTF8(output);
  //std::cout << result << std::endl;
  //return nullptr;
}

std::string runPythonCode(std::string_view pycode)
{
  PyObject* sys = nullptr;
  PyObject* io = nullptr;
  PyObject* string_io = nullptr;
  PyObject* output = nullptr;

  D_ASSERT(Py_IsInitialized());

  // Create objects for redirection
  sys = PyImport_ImportModule("sys");
  io = PyImport_ImportModule("io");

  // Create a new StringIO object
  string_io = PyObject_CallMethod(io, "StringIO", nullptr);
  if (string_io == nullptr)
  {
    PyErr_Print();
    return "";
  }

  // Redirect sys.stdout and sys.stderr to StringIO
  PyObject_SetAttrString(sys, "stdout", string_io);
  PyObject_SetAttrString(sys, "stderr", string_io);

  //const char* code = "print('Hello from Python!')\nprint(1 + 2)";

  PyRun_SimpleString(pycode.data());

  // Get the output from the StringIO object
  output = PyObject_CallMethod(string_io, "getvalue", nullptr);
  std::string result = PyUnicode_AsUTF8(output);

  // Clean up
  Py_DECREF(output);
  Py_DECREF(string_io);
  Py_DECREF(io);
  Py_DECREF(sys);
  return result;
}

// int runPythonScript(char const* fileName)
// {
//   D_ASSERT_MSG(Py_IsInitialized(), "No 'Py_Initialize()' call.");
//   D_ASSERT(checkPySysPath(fileName));

//   FILE* fp = ::fopen(fileName, "rb");
//   if (!fp)
//     D_THROW("unable to read Python script '" + std::string(fileName) + std::string("': ")
//             + std::strerror(errno));

//   int result = PyRun_SimpleFile(fp, fileName);
//   ::fclose(fp);
//   return result;
// }

}  // namespace DPYTHON
