#include "libd/libdpython/pythoninterface.h"
#include "tests/testbase.h"

#include <filesystem>
#include <iostream>

namespace {
class PythonScriptTests : public TestBase
{};
}  // namespace

TEST_F(PythonScriptTests, Test_VenvRunningSimplePythonCodeAndCaptureOutput)
{
  auto cwd = std::filesystem::current_path();
  auto venv = cwd / std::filesystem::path(".venv/bin/python3");

  DPYTHON::openVenvEnvironment(venv.string());
  ASSERT_TRUE(Py_IsInitialized());

  auto pyhtonCode
      = "print('Hello from Python!')\n"
        "print('Simple Calculation:')\n"
        "result = 1+2\n"
        "print('1+2=' + str(result))";

  auto capturedOut = DPYTHON::runPythonCode(pyhtonCode);

  ASSERT_THAT(capturedOut, testing::HasSubstr("Hello from Python!"));
  ASSERT_THAT(capturedOut, testing::HasSubstr("Simple Calculation:"));
  ASSERT_THAT(capturedOut, testing::HasSubstr("1+2=3"));

  DPYTHON::closePythonEnvironment();
  ASSERT_FALSE(Py_IsInitialized());
}

TEST_F(PythonScriptTests, Test_VenvYFinanceImport)
{
  auto cwd = std::filesystem::current_path();
  auto venv = cwd / std::filesystem::path(".venv/bin/python3");

  DPYTHON::openVenvEnvironment(venv.string());
  ASSERT_TRUE(Py_IsInitialized());

  auto pyhtonCode
      = "import yfinance\n"
        "from datetime import datetime, timedelta\n"
        "ticker = 'AAPL'\n"
        "company = yfinance.Ticker(ticker)\n"
        "company_name = company.info.get('longName', 'Name not available')\n"
        "print('Stock ticker: ' + ticker)\n"
        "print('Company Name: ' + company_name)\n";

  auto capturedOut = DPYTHON::runPythonCode(pyhtonCode);

  ASSERT_THAT(capturedOut, testing::Not(testing::HasSubstr("Traceback (most recent call last):")));
  ASSERT_THAT(capturedOut, testing::HasSubstr("AAPL"));
  ASSERT_THAT(capturedOut, testing::HasSubstr("Apple Inc."));

  DPYTHON::closePythonEnvironment();
  ASSERT_FALSE(Py_IsInitialized());
}

TEST_F(PythonScriptTests, Test_VenvRunPyhtonCodeModuleNotFound)
{
  auto cwd = std::filesystem::current_path();
  auto venv = cwd / std::filesystem::path(".venv/bin/python3");

  DPYTHON::openVenvEnvironment(venv.string());
  ASSERT_TRUE(Py_IsInitialized());

  auto pyhtonCode
      = "import bliblabluf\n"
        "print('That module does not exist!')";

  auto capturedOut = DPYTHON::runPythonCode(pyhtonCode);
  ASSERT_THAT(capturedOut, testing::HasSubstr("Traceback (most recent call last):"));

  DPYTHON::closePythonEnvironment();
  ASSERT_FALSE(Py_IsInitialized());
}

TEST_F(PythonScriptTests, Test_VenvRunPyhtonFile) {}

TEST_F(PythonScriptTests, testSomethingForException)
{
  //W_EXPECT_THROW("do something exceptional", "something bad is going on");
}
