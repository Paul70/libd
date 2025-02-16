#ifndef LIBD_DPYTHON_PYTHONINTERFACE_H
#define LIBD_DPYTHON_PYTHONINTERFACE_H
#include <Python.h>
#include <filesystem>
#include <string_view>

namespace DUTIL {
class Settings;
}  // namespace DUTIL

namespace DPYTHON {

/*! \brief Initializes Python system runtime environment.
 *
 * Sets up the pyhton system interpreter and initializes all
 * essential components.
 * In general, one have to call this function only once until the program
 * finishes execution or a PyFinalize() call was done.
 *
 * Throws an exception in case there is no Python system installation.
 *
 * @param log: logger instance
 */
void openSystemEnvironment();

/*! \brief Initializes specific Python virtual runtime environment.
 *
 * Sets up the pyhton venv interpreter and initializes all
 * essential components.
 * In general, one have to call this function only once until the program
 * finishes execution or a PyFinalize() call was done.
 *
 * Throws an exception in case the specified Python venv does not exist.
 *
 * @param venvPath: file system path to the venv's python interpreter
 *        executable. Relative paths are evaluated with respect to the project's
 *        runtime directoroy.
 * @param log: logger instance
 */
void openVenvEnvironment(std::string_view venvPaht);

/*! \brief Closes Python runtime environment.
 *
 * After clsoing a runtime environment, a reinitialization is necessary
 * to execute Python code again.
 *
 * @param log: logger instance
 */
void closePythonEnvironment() noexcept;

/*! \brief Appends the given path string to the system path variable
 *  PATH of the initialized Python runtime environment.
 *
 * @param log: logger instance
 */
void appendSysPaht(std::string_view path);

/*! \brief Functions that runs specified Python code.
 *
 *  Prerequesites:
 *  - a Python environment is open and initialized
 *  - the Python file location is part of the environment's SYSPATH variable
 *
 * The function is able to have a std::string input parameter meant to hold a
 * serialized data transfer object (dto). Similarly, it returns a string
 * which can also be a serialzed dto.
 * It is possible to export captured std_cout and std_err cli output of the executed
 * pyhton code via the shared string pointer paramter.
 *
 * @param pyfile: Python file containing the code to be executed.
 * @param method: Pyhton method to be executed, if empty, whole python file will be executed.
 * @param dto: std::string containing a serialized data transfer object  which will be deserialized
 *        in the python code.
 * @param capturedCli: Reference to a string object for exporting the caputred cli output of the executed
 *        python code.
 * @return string return object.
 */
std::string runPythonCode(std::filesystem::path file, std::string_view method, std::string dto,
                          std::shared_ptr<std::string> outputCli = nullptr);

/*! \brief Functions that runs Python code given as simple string and captures the output.
 *
 *  Prerequesites:
 *  - a Python environment is open and initialized
 *
 *  Function executes python code given as simple text string and captures the output by
 *  redirecting Pythons sys.stderr and sys.stdout of sys module to a custom implementation.
 *
 * @param pycode: Python code given as a plain character text string.
 * @return text string containing the captured output.
 */
std::string runPythonCode(std::string_view pycode);

//! Execute a Python script given by file.
//int runPythonScript(char const* fileName);

}  // namespace DPYTHON
#endif  // LIBD_DPYTHON_PYTHONINTERFACE_H
