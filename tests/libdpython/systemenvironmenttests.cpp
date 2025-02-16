#include "libdpython/pythoninterface.h"
#include "tests/testbase.h"

#include <codecvt>
#include <filesystem>
#include <locale>

namespace {
class SystemEnvironmentTests : public TestBase
{};
}  // namespace

TEST_F(SystemEnvironmentTests, Test_OpenSystemPythonEnvironment)
{
  DPYTHON::openSystemEnvironment();
  ASSERT_TRUE(Py_IsInitialized());

  std::string sys_prefix{"usr/bin/python3"};
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  auto path = Py_GetProgramFullPath();

  ASSERT_THAT(converter.to_bytes(path), testing::HasSubstr(sys_prefix));

  DPYTHON::closePythonEnvironment();
  ASSERT_FALSE(Py_IsInitialized());
}

TEST_F(SystemEnvironmentTests, Test_OpenVirtualPythonEnvrionment)
{
  auto cwd = std::filesystem::current_path();
  auto venv = cwd / std::filesystem::path(".venv/bin/python3");

  DPYTHON::openVenvEnvironment(venv.c_str());
  ASSERT_TRUE(Py_IsInitialized());

  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  ASSERT_THAT(converter.to_bytes(Py_GetProgramFullPath()), testing::HasSubstr(venv));

  DPYTHON::closePythonEnvironment();
  ASSERT_FALSE(Py_IsInitialized());
}

// auto result = PyRun_SimpleString(
//     "import sys; "
//     "sys_info = None"
//     "sys_info[sys.executable] = "
//     "return "
//     "print(f'{sys.executable=}\\n{sys.prefix=}\\n{sys.exec_prefix=}\\n{sys.base_prefix=}\\n{sys."
//     "base_exec_prefix=}\\n{sys.path=}')");
