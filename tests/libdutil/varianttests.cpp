#include "libdutil/basictypes.h"
#include "libdutil/variant.h"
#include "tests/testbase.h"
#include "tests/testdummy.h"
#include <iostream>

using namespace DUTIL;
using namespace DUTIL::VariantDetail;

namespace {
class VariantTests : public TestBase
{};
} // namespace

TEST_F(VariantTests, testVariantConstructionWorksAsExpected)
{
    // construction of monostate variant
    {
        Variant var;
        EXPECT_TRUE(var.isMonostate());
        EXPECT_FALSE(var.isString());
        EXPECT_FALSE(var.isNumeric());
        EXPECT_FALSE(var.isCharacter());
        EXPECT_FALSE(var.isBool());
    }
    // construction of string variant
    {
        Variant var("Hello World!");
        EXPECT_FALSE(var.isMonostate());
        EXPECT_TRUE(var.isString());
        EXPECT_FALSE(var.isNumeric());
        EXPECT_FALSE(var.isCharacter());
        EXPECT_FALSE(var.isBool());
    }
    // construction of std::int64_t variant
    {
        Variant var(std::int64_t(123));
        Variant var_neg(std::int64_t(-123));
        EXPECT_FALSE(var.isMonostate());
        EXPECT_FALSE(var.isString());
        EXPECT_TRUE(var.isNumeric());
        EXPECT_FALSE(var.isCharacter());
        EXPECT_FALSE(var.isBool());
        EXPECT_FALSE(var_neg.isMonostate());
        EXPECT_FALSE(var_neg.isString());
        EXPECT_TRUE(var_neg.isNumeric());
        EXPECT_FALSE(var_neg.isCharacter());
        EXPECT_FALSE(var_neg.isBool());
    }
    // construct of int alias DUTIL::label_t variant
    {
        Variant var(label_t(1234567));
        EXPECT_FALSE(var.isString());
        EXPECT_FALSE(var.isCharacter());
        EXPECT_FALSE(var.isBool());
        EXPECT_FALSE(var.isMonostate());
        EXPECT_TRUE(var.isNumeric());
    }

    // construction of std::uint64_t variant
    {
        Variant var(std::uint64_t(123));
        EXPECT_FALSE(var.isMonostate());
        EXPECT_FALSE(var.isString());
        EXPECT_TRUE(var.isNumeric());
        EXPECT_FALSE(var.isCharacter());
        EXPECT_FALSE(var.isBool());
    }
    // construction of double variant
    {
        Variant var(123.456);
        Variant var_neg(123.456);
        Variant var_real(real_t(1234.56789));
        EXPECT_FALSE(var.isMonostate());
        EXPECT_FALSE(var.isString());
        EXPECT_TRUE(var.isNumeric());
        EXPECT_FALSE(var.isCharacter());
        EXPECT_FALSE(var.isBool());
        EXPECT_FALSE(var_neg.isMonostate());
        EXPECT_FALSE(var_neg.isString());
        EXPECT_TRUE(var_neg.isNumeric());
        EXPECT_FALSE(var_neg.isCharacter());
        EXPECT_FALSE(var_neg.isBool());
        EXPECT_FALSE(var_real.isMonostate());
        EXPECT_FALSE(var_real.isString());
        EXPECT_TRUE(var_real.isNumeric());
        EXPECT_FALSE(var_real.isCharacter());
        EXPECT_FALSE(var_real.isBool());
    }
    // construction of bool variant
    {
        Variant var(true);
        EXPECT_FALSE(var.isMonostate());
        EXPECT_FALSE(var.isString());
        EXPECT_FALSE(var.isNumeric());
        EXPECT_FALSE(var.isCharacter());
        EXPECT_TRUE(var.isBool());
    }
    // construction of char variant
    {
        Variant var('A');
        EXPECT_FALSE(var.isMonostate());
        EXPECT_FALSE(var.isString());
        EXPECT_FALSE(var.isNumeric());
        EXPECT_TRUE(var.isCharacter());
        EXPECT_FALSE(var.isBool());
    }
    {
        Variant v1("Hallo");
        Variant v2 = v1;
        Variant v3(v2);
        EXPECT_TRUE(v2.isString());
        EXPECT_TRUE(v3.isString());
    }

    // construction of Variant holding a NamedEnum
    {
        using namespace LIBD;
        using namespace TESTS;

        // define a NamedEnum object.
        TestDummy::COLOR col = TestDummy::COLOR::GREEN;
        Variant var{col};
        Variant var2{TestDummy::COLOR::GREEN};

        auto type = var.getType();
        EXPECT_TRUE(type == Variant::Type::STRING);
        type = var2.getType();
        EXPECT_TRUE(type == Variant::Type::STRING);
        EXPECT_EQ(var.toString(), "GREEN");
        EXPECT_EQ(var2.toString(), "GREEN");
    }
}

TEST_F(VariantTests, testGetAsWorksAsExpected)
{
    // Case: Variant holding double
    // Types to test: std::int64_t, std::uint64_t, bool, char, std::string
    {
        Variant var(2.345);
        auto resultA = var.getAs<std::int64_t>(); // as std::int64_t
        auto type_test = std::is_same_v<std::int64_t, decltype(resultA.second)>;
        EXPECT_EQ(2, resultA.second);
        EXPECT_TRUE(type_test);
        auto resultB = var.getAs<std::uint64_t>(); // as std::uint64_t
        type_test = std::is_same_v<std::uint64_t, decltype(resultB.second)>;
        EXPECT_EQ(2, resultB.second);
        EXPECT_TRUE(type_test);
        auto resultC = var.getAs<bool>(); // as bool
        type_test = std::is_same_v<bool, decltype(resultC.second)>;
        EXPECT_EQ(true, resultC.second);
        EXPECT_TRUE(type_test);
        auto resultD = var.getAs<char>(); // as character
        type_test = std::is_same_v<char, decltype(resultD.second)>;
        EXPECT_EQ(0x02, resultD.second);
        EXPECT_TRUE(resultD.first);
        EXPECT_TRUE(type_test);
        auto resultE = var.getAs<std::string>(); // as std::string
        type_test = std::is_same_v<std::string, decltype(resultE.second)>;
        EXPECT_EQ("2.3450000000", resultE.second);
        EXPECT_TRUE(resultE.first);
        EXPECT_TRUE(type_test);

        // test for label type defined in basictyped.h
        auto resultF = var.getAs<label_t>();
        EXPECT_EQ(2, resultF.second);
        type_test = std::is_same_v<label_t, decltype(resultF.second)>;
        EXPECT_TRUE(type_test);
    }

    // Case: Variant holding std::int64_t
    // Types to test: std::uint64_t, double, bool, char, std::string
    {
        Variant var(std::int64_t(777777));
        auto resultA = var.getAs<std::uint64_t>(); // as std::uint64_t
        auto type_test = std::is_same_v<std::uint64_t, decltype(resultA.second)>;
        EXPECT_EQ(777777, resultA.second);
        EXPECT_TRUE(type_test);
        auto resultB = var.getAs<double>(); // as double
        type_test = std::is_same_v<double, decltype(resultB.second)>;
        EXPECT_EQ(777777.0, resultB.second);
        EXPECT_TRUE(type_test);
        auto resultB2 = var.getAs<real_t>(); // as double
        type_test = std::is_same_v<real_t, decltype(resultB2.second)>;
        EXPECT_EQ(777777.0, resultB2.second);
        EXPECT_TRUE(type_test);
        auto resultC = var.getAs<bool>(); // as bool
        type_test = std::is_same_v<bool, decltype(resultC.second)>;
        EXPECT_EQ(true, resultC.second);
        EXPECT_TRUE(type_test);
        auto resultD = var.getAs<char>(); // as character
        type_test = std::is_same_v<char, decltype(resultD.second)>;
        EXPECT_EQ('1', resultD.second);
        EXPECT_TRUE(resultD.first);
        EXPECT_TRUE(type_test);
        auto resultE = var.getAs<std::string>(); // as std::string
        type_test = std::is_same_v<std::string, decltype(resultE.second)>;
        EXPECT_EQ("777777", resultE.second);
        EXPECT_TRUE(resultE.first);
        EXPECT_TRUE(type_test);

        auto resultF = var.getAs<label_t>();
        EXPECT_EQ(777777, resultF.second);
        type_test = std::is_same_v<label_t, decltype(resultF.second)>;
        EXPECT_TRUE(type_test);
    }

    {
        Variant var(std::int64_t(-4597));
        auto resultA = var.getAs<std::uint64_t>(); // as std::uint64_t
        EXPECT_EQ(4597, resultA.second);
        EXPECT_TRUE(resultA.first);
        auto resultB = var.getAs<double>(); // as double
        auto type_test = std::is_same_v<double, decltype(resultB.second)>;
        EXPECT_EQ(-4597.0, resultB.second);
        EXPECT_TRUE(type_test);
        auto resultC = var.getAs<bool>(); // as bool
        type_test = std::is_same_v<bool, decltype(resultC.second)>;
        EXPECT_EQ(true, resultC.second);
        EXPECT_TRUE(type_test);
        auto resultD = var.getAs<char>(); // as character
        type_test = std::is_same_v<char, decltype(resultD.second)>;
        EXPECT_EQ(0x0b, resultD.second);
        EXPECT_TRUE(resultD.first);
        EXPECT_TRUE(type_test);
        auto resultE = var.getAs<std::string>(); // as std::string
        type_test = std::is_same_v<std::string, decltype(resultE.second)>;
        EXPECT_EQ("-4597", resultE.second);
        EXPECT_TRUE(resultE.first);
        EXPECT_TRUE(type_test);
        auto resultF = var.getAs<label_t>();
        EXPECT_EQ(-4597, resultF.second);
        type_test = std::is_same_v<label_t, decltype(resultF.second)>;
        EXPECT_TRUE(type_test);
    }

    // Case: Variant holding std::uint64_t
    // Types to test: std::int64_t, double, bool, char, std::string
    {
        Variant var(std::uint64_t(12345));
        auto resultA = var.getAs<std::int64_t>(); // as std::int64_t
        auto type_test = std::is_same_v<std::int64_t, decltype(resultA.second)>;
        EXPECT_EQ(12345, resultA.second);
        EXPECT_TRUE(resultA.first);
        EXPECT_TRUE(type_test);
        auto resultB = var.getAs<double>(); // as double
        type_test = std::is_same_v<double, decltype(resultB.second)>;
        EXPECT_EQ(12345.0, resultB.second);
        EXPECT_TRUE(type_test);
        auto resultC = var.getAs<bool>(); // as bool
        type_test = std::is_same_v<bool, decltype(resultC.second)>;
        EXPECT_EQ(true, resultC.second);
        EXPECT_TRUE(type_test);
        auto resultE = var.getAs<std::string>(); // as std::string
        type_test = std::is_same_v<std::string, decltype(resultE.second)>;
        EXPECT_EQ("12345", resultE.second);
        EXPECT_TRUE(resultE.first);
        EXPECT_TRUE(type_test);
        auto resultF = var.getAs<label_t>();
        EXPECT_EQ(12345, resultF.second);
        type_test = std::is_same_v<label_t, decltype(resultF.second)>;
        EXPECT_TRUE(type_test);
    }

    // Case: Variant holding std::string
    // Types to test: std::uint64_t, std::int64_t, double, bool
    {
        Variant var("1234.567");
        auto resultA = var.getAs<double>(); // as double
        auto type_test = std::is_same_v<double, decltype(resultA.second)>;
        EXPECT_EQ(1234.567, resultA.second);
        EXPECT_TRUE(resultA.first);
        EXPECT_TRUE(type_test);
        auto resultB = var.getAs<std::int64_t>(); // as double
        type_test = std::is_same_v<std::int64_t, decltype(resultB.second)>;
        EXPECT_EQ(1234, resultB.second);
        EXPECT_TRUE(resultB.first);
        EXPECT_TRUE(type_test);
        auto resultC = var.getAs<std::uint64_t>(); // as double
        type_test = std::is_same_v<std::uint64_t, decltype(resultC.second)>;
        EXPECT_EQ(1234, resultC.second);
        EXPECT_TRUE(resultC.first);
        EXPECT_TRUE(type_test);
        auto resultD = var.getAs<bool>(); // as double
        type_test = std::is_same_v<bool, decltype(resultD.second)>;
        EXPECT_EQ(true, resultD.second);
        EXPECT_TRUE(resultD.first);
        EXPECT_TRUE(type_test);
        auto resultF = var.getAs<label_t>();
        EXPECT_EQ(1234, resultF.second);
        type_test = std::is_same_v<label_t, decltype(resultF.second)>;
        EXPECT_TRUE(type_test);
    }
    {
        Variant var("-1234.567");
        auto resultA = var.getAs<double>(); // as double
        auto type_test = std::is_same_v<double, decltype(resultA.second)>;
        EXPECT_EQ(-1234.567, resultA.second);
        EXPECT_TRUE(resultA.first);
        EXPECT_TRUE(type_test);
        auto resultC = var.getAs<std::uint64_t>(); // as double
        type_test = std::is_same_v<std::uint64_t, decltype(resultC.second)>;
        EXPECT_EQ(1234, resultC.second);
        EXPECT_TRUE(resultC.first);
        EXPECT_TRUE(type_test);
    }
}

TEST_F(VariantTests, testToStringWorksAsExpected)
{
    Variant var(2.345);
    std::string result = var.toString();
    EXPECT_EQ("2.3450000000", result);
}

TEST_F(VariantTests, testComparisonOperatorWorksAsExpected)
{
    Variant var1(2.345);
    Variant var2(2.345);
    Variant var3(123.123);
    Variant var4("Hallo");
    Variant var5("Hallo");
    ASSERT_EQ(var1, var2);
    ASSERT_EQ(var4, var5);
    ASSERT_EQ(var2, var1);
    ASSERT_EQ(var5, var4);
    ASSERT_NE(var2, var3);
    ASSERT_NE(var1, var4);
    ASSERT_NE(var1, var5);
    ASSERT_NE(var2, var4);
    ASSERT_NE(var3, var4);
    ASSERT_NE(var4, var1);
    ASSERT_NE(var4, var3);
}

TEST_F(VariantTests, testIsValid)
{
    Variant v0 = Variant();
    Variant v1(2);
    Variant v2(2.3);
    Variant v3("dd");
    Variant v4('d');
    Variant v5(true);
    ASSERT_FALSE(v0.isValid());
    ASSERT_TRUE(v1.isValid());
    ASSERT_TRUE(v2.isValid());
    ASSERT_TRUE(v3.isValid());
    ASSERT_TRUE(v4.isValid());
    ASSERT_TRUE(v5.isValid());
}

TEST_F(VariantTests, testConvertTo)
{
    Variant v(2.3);
    v.convertTo(Variant::Type::STRING);
    ASSERT_TRUE(v.isString());
    auto result = v.getAs<std::string>().second;
    Utility::trimZeros(result);
    ASSERT_EQ("2.3", result);
    v.convertTo(Variant::Type::LABEL);
    ASSERT_TRUE(v.isNumeric());
    ASSERT_EQ(2, v.getAs<label_t>().second);
    v.convertTo(Variant::Type::DOUBLE);
    ASSERT_TRUE(v.isNumeric());
    auto result2 = v.getAs<real_t>().second;
    ASSERT_EQ(2.0, result2);
    v.convertTo(Variant::Type::BOOL);
    ASSERT_TRUE(v.isBool());
    ASSERT_EQ(true, v.getAs<bool>().second);
    v.convertTo(Variant::Type::STRING);
    ASSERT_TRUE(v.isString());
    result = v.getAs<std::string>().second;
    Utility::trimZeros(result);
    ASSERT_EQ("2.0", result);
    v.convertTo(Variant::Type::MONOSTATE);
    ASSERT_TRUE(v.isMonostate());
}
