#ifndef LIBD_TESTS_TESTBASE_H
#define LIBD_TESTS_TESTBASE_H
#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>

class TestBase : public ::testing::Test
{
public:
    /*! \brief Return the directory containing any permanent files used in the tests.
     *
     * The location of the directory is taken from the preprocessor macro "TEST_DATA_DIR".
     * If this directory does not exist, attempt to look in "../../test_data".
     * If that fails as well, trigger an exception.
     */
    ///static std::string testDataSourceDir();

    static std::size_t getNumberOfRegisteredConcreteClasses() noexcept;
    static std::size_t getNumberOfRegisteredInterfaces() noexcept;
    static std::size_t getNumberOfConcreteClassesForInterface(std::string const &interface) noexcept;

    //std::cout << e.what() << std::endl;
protected:
    //! Read the given file into the result string.
    //static bool slurpFile(std::string const& fileName, std::string& result);

}; // TestBase

#define D_EXPECT_THROW(statement, substring) \
[&] () \
{ \
    try { \
        statement; \
        ADD_FAILURE() << "No exception thrown."; \
        return "No exception thrown."; \
    } catch (Exception const &e) { \
        std::string error_msg = e.what(); \
        if (error_msg.rfind(substring) == std::string::npos) { \
            std::string str = "Unexpected exception message thrown: " + error_msg + std::string(" Expected message was: ") \
                              + #substring; \
            ADD_FAILURE() << str.data(); \
        } \
        return substring; \
    } \
} ()

#endif // LIBD_TESTS_TESTBASE_H
