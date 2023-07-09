#include "libdutil/utility.h"
#include "tests/testbase.h"

using namespace DUTIL;
using namespace Utility;

namespace {
class UtilityTests : public TestBase
{};
} // namespace

TEST_F(UtilityTests, tesReplaceDecimalSeperatorWorksAsExpected)
{
    std::string numA{"123,456"};
    std::string numResult{"123.456"};
    std::string noNumber{"Hello World!"};
    replaceDecimalSeperator(',', '.', numA);
    replaceDecimalSeperator(',', '.', numResult);
    EXPECT_EQ(numA, numResult);
    replaceDecimalSeperator('.', ',', numResult);
    EXPECT_EQ("123,456", numResult);
    replaceDecimalSeperator('.', ',', noNumber);
    EXPECT_EQ("Hello World!", noNumber);
    replaceDecimalSeperator('.', ',', noNumber);
    EXPECT_EQ("Hello World!", noNumber);
}

TEST_F(UtilityTests, testIntegerToStringInt64InputWorksAsExpected)
{
    int numA = 123;
    EXPECT_EQ(integerToString(numA), "123");
    int numB = 2147483647; // which is (2^31)-1, largest number representable by int
    EXPECT_EQ(integerToString(numB), "2147483647");
    std::int64_t numC = 4294967296;
    EXPECT_EQ(integerToString(numC), "4294967296");
    int numD = -123;
    EXPECT_EQ(integerToString(numD), "-123");
    int numE = -2147483647; // which is (2^31)-1, largest number representable by int
    EXPECT_EQ(integerToString(numE), "-2147483647");
}

TEST_F(UtilityTests, testDoubleToStringWorksAsExpected)
{
    double numA = 2.7;
    EXPECT_EQ(doubleToString(numA), "2.7000000000"); // default precision is 10
    double numB = 2.7;
    EXPECT_EQ(doubleToString(numB, 1), "2.7");
    double numC = 1234562.778964;
    EXPECT_EQ(doubleToString(numC, 8), "1234562.77896400");
    double numD = -1234562.778964;
    EXPECT_EQ(doubleToString(numD, 8), "-1234562.77896400");
}

TEST_F(UtilityTests, testArithmeticToStringWorksAsExpected)
{
    double numA = 2.7;
    EXPECT_EQ(arithmeticToString(numA), "2.7000000000"); // default precision is 10
    double numB = 2.7;
    EXPECT_EQ(arithmeticToString(numB, 1), "2.7");
    double numC = 1234562.778964;
    EXPECT_EQ(arithmeticToString(numC, 8), "1234562.77896400");
    double numD = -1234562.778964;
    EXPECT_EQ(arithmeticToString(numD, 8), "-1234562.77896400");
    uint numE = 12234;
    EXPECT_EQ(arithmeticToString(numE), "12234");
    bool numF = true;
    EXPECT_EQ(arithmeticToString(numF), "1");
    char numG = 'P'; // ASCII: 80
    EXPECT_EQ(arithmeticToString(numG), "80");
}

TEST_F(UtilityTests, testStringToArithmeticWorksAsExpedted)
{
    std::string str{"1234.5678"};
    EXPECT_EQ(1234.5678, stringToArithmetic<double>(str));
    EXPECT_EQ(1234, stringToArithmetic<int>(str));
    EXPECT_EQ(1234, stringToArithmetic<std::int64_t>(str));
    EXPECT_EQ(1, stringToArithmetic<bool>(str));

    str = "-1234.5678";
    EXPECT_EQ(-1234.5678, stringToArithmetic<double>(str));
    EXPECT_EQ(-1234, stringToArithmetic<int>(str));
    EXPECT_EQ(1234, stringToArithmetic<std::uint64_t>(str));

    str = "Hello World!";
    D_EXPECT_THROW(stringToArithmetic<double>(str), "stod");
}

TEST_F(UtilityTests, testTrimStringWorksAsExpected)
{
    std::string str("   Im Westen nichts Neues     ");
    auto s1 = trim(str);
    EXPECT_EQ("Im Westen nichts Neues", s1);
    trimThis(str);
    EXPECT_EQ("Im Westen nichts Neues", str);
    std::string str2 = "     Hallo";
    s1 = trimMove(std::move(str2));
    EXPECT_EQ("Hallo", s1);
}

TEST_F(UtilityTests, testSplitWorksAsExpected)
{
    std::string str("Im Westen nichts Neues");
    auto list = split(str);
    EXPECT_TRUE(list.empty());
    str = ("RED = 5, BLUE = 10, GREEN");
    list = split(str);
    EXPECT_TRUE(list.size() == 3);
    EXPECT_EQ("RED = 5", list.front());
    EXPECT_EQ("BLUE = 10", list[1]);
    EXPECT_EQ("GREEN", list[2]);

    str = "1;2;3;4;5;6;7;8;9";
    list = split(str, ';');
    EXPECT_TRUE(list.size() == 9);
    EXPECT_EQ("1", list.front());
    EXPECT_EQ("2", list[1]);
    EXPECT_EQ("3", list[2]);
    EXPECT_EQ("9", list.back());

    str = "RED = 5,   BLUE = 10   ,                 GREEN";
    list = split(str);
    EXPECT_TRUE(list.size() == 3);
    EXPECT_EQ("RED = 5", list.front());
    EXPECT_EQ("BLUE = 10", list[1]);
    EXPECT_EQ("GREEN", list[2]);
}

TEST_F(UtilityTests, testTrimZeros)
{
    std::string s{"23.34000000"};
    trimZeros(s);
    ASSERT_EQ("23.34", s);
    s = "2.56700030000";
    trimZeros(s);
    ASSERT_EQ("2.5670003", s);
    s = "0.0";
    trimZeros(s);
    ASSERT_EQ("0.0", s);
}
