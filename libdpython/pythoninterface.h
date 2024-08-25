#ifndef LIBD_DPYTHON_PYTHONINTERFACE_H
#define LIBD_DPYTHON_PYTHONINTERFACE_H
#include <Python.h>
#include <string>
#include <vector>

namespace LIBD::DPYTHON {

/*! \brief description of PythonInterface
 *
 * Longer description of PythonInterface.
 */

void openPythonEnvironment();

void closePythonEnvironment();
//! Add a path to the python system path variable.
void appendSysPaht(char const* path);

//! Execute a Python script given by file.
int runPythonScript(char const* fileName);

int runPythonModuleMethod(char const* fileName, char const* methodName,
                          std::vector<std::string> const& argv);

}  // namespace LIBD::DPYTHON
#endif  // LIBD_DPYTHON_PYTHONINTERFACE_H
