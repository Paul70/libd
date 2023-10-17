#include "libd/libdutil/constructiondata.h"
#include "libd/libdutil/dataset.h"
#include "libd/tests/testbase.h"

using namespace DUTIL;

namespace {
class DatasetTests : public TestBase
{};
} // namespace

TEST_F(DatasetTests, TypeMap_forKnownTypes_returnsExpectedResult)
{
    ASSERT_EQ(Dataset::Type::INT8, Dataset::TypeMap<int8_t>());
    ASSERT_EQ(Dataset::Type::UINT8, Dataset::TypeMap<uint8_t>());
    ASSERT_EQ(Dataset::Type::INT16, Dataset::TypeMap<int16_t>());
    ASSERT_EQ(Dataset::Type::UINT16, Dataset::TypeMap<uint16_t>());
    ASSERT_EQ(Dataset::Type::INT32, Dataset::TypeMap<int32_t>());
    ASSERT_EQ(Dataset::Type::UINT32, Dataset::TypeMap<uint32_t>());
    ASSERT_EQ(Dataset::Type::INT64, Dataset::TypeMap<int64_t>());
    ASSERT_EQ(Dataset::Type::UINT64, Dataset::TypeMap<uint64_t>());
    ASSERT_EQ(Dataset::Type::FLOAT32, Dataset::TypeMap<float>());
    ASSERT_EQ(Dataset::Type::FLOAT64, Dataset::TypeMap<double>());
}

TEST_F(DatasetTests, TypeMap_forLong_returnsEitherInt64OrInt32)
{
    ASSERT_TRUE(Dataset::Type::INT32 == Dataset::TypeMap<long>() || Dataset::Type::INT64 == Dataset::TypeMap<long>());
}

TEST_F(DatasetTests, defaultConstruct_propertiesAsExpected)
{
    Dataset dt;
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(0, dt.getRows());
    ASSERT_EQ(Dataset::Type::EMPTY, dt.getType());
    ASSERT_EQ(std::vector<signed char>(), dt.getValues<signed char>(0));
}

TEST_F(DatasetTests, constructFromVectorOfInt8_t_propertiesAsExpected)
{
    std::vector<int8_t> v(12, int8_t(-5));
    Dataset dt(v);
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(12, dt.getRows());
    ASSERT_EQ(Dataset::Type::INT8, dt.getType());
    ASSERT_EQ(v, dt.getValues<int8_t>());
}

TEST_F(DatasetTests, constructFromVectorOfInt8_t_with_specified_nCols_propertiesAsExpected)
{
    std::vector<int8_t> v(12, int8_t(-5));
    Dataset dt(v, 3);
    ASSERT_EQ(3, dt.getCols());
    ASSERT_EQ(4, dt.getRows());
    ASSERT_EQ(Dataset::Type::INT8, dt.getType());
    ASSERT_EQ(v, dt.getValues<int8_t>());
}

TEST_F(DatasetTests, constructFromVectorOfUInt8_t_propertiesAsExpected)
{
    std::vector<uint8_t> v(3, int8_t(33));
    Dataset dt(v);
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(3, dt.getRows());
    ASSERT_EQ(Dataset::Type::UINT8, dt.getType());
    ASSERT_EQ(v, dt.getValues<uint8_t>());
}

TEST_F(DatasetTests, constructFromVectorOfInt16_t_propertiesAsExpected)
{
    std::vector<int16_t> v(10, int16_t(-500));
    Dataset dt(v);
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(10, dt.getRows());
    ASSERT_EQ(Dataset::Type::INT16, dt.getType());
    ASSERT_EQ(v, dt.getValues<int16_t>());
}

TEST_F(DatasetTests, constructFromVectorOfUInt16_t_propertiesAsExpected)
{
    std::vector<uint16_t> v(4, int16_t(300));
    Dataset dt(v);
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(4, dt.getRows());
    ASSERT_EQ(Dataset::Type::UINT16, dt.getType());
    ASSERT_EQ(v, dt.getValues<uint16_t>());
}

TEST_F(DatasetTests, constructFromVectorOfInt32_t_propertiesAsExpected)
{
    std::vector<int32_t> v(5, int32_t(-2000000000));
    Dataset dt(v);
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(5, dt.getRows());
    ASSERT_EQ(Dataset::Type::INT32, dt.getType());
    ASSERT_EQ(v, dt.getValues<int32_t>());
}

TEST_F(DatasetTests, constructFromVectorOfUInt32_t_propertiesAsExpected)
{
    std::vector<uint32_t> v(1, uint32_t(4294967295));
    Dataset dt(v);
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(1, dt.getRows());
    ASSERT_EQ(Dataset::Type::UINT32, dt.getType());
    ASSERT_EQ(v, dt.getValues<uint32_t>());
}

TEST_F(DatasetTests, constructFromVectorOfInt64_t_propertiesAsExpected)
{
    std::vector<int64_t> v(3, int64_t(-9223372036854775807));
    Dataset dt(v);
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(3, dt.getRows());
    ASSERT_EQ(Dataset::Type::INT64, dt.getType());
    ASSERT_EQ(v, dt.getValues<int64_t>());
}

TEST_F(DatasetTests, constructFromVectorOfUInt64_t_propertiesAsExpected)
{
    std::vector<uint64_t> v(1, uint64_t(18446744073709551615U));
    Dataset dt(v);
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(1, dt.getRows());
    ASSERT_EQ(Dataset::Type::UINT64, dt.getType());
    ASSERT_EQ(v, dt.getValues<uint64_t>());
}

TEST_F(DatasetTests, constructFromVectorOfFloat_propertiesAsExpected)
{
    std::vector<float> v(3, -42.42f);
    Dataset dt(v);
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(3, dt.getRows());
    ASSERT_EQ(Dataset::Type::FLOAT32, dt.getType());
    ASSERT_EQ(v, dt.getValues<float>());
}

TEST_F(DatasetTests, constructFromVectorOfDouble_propertiesAsExpected)
{
    std::vector<double> v(3, -42.42);
    Dataset dt(v);
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(3, dt.getRows());
    ASSERT_EQ(Dataset::Type::FLOAT64, dt.getType());
    ASSERT_EQ(v, dt.getValues<double>());
}

TEST_F(DatasetTests, moveConstructFromVectorOfInt8_t_propertiesAsExpected)
{
    std::vector<int8_t> v(12, int8_t(-5));
    ASSERT_EQ(12u, v.size());
    Dataset dt(std::move(v));
    ASSERT_EQ(0u, v.size());
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(12, dt.getRows());
    ASSERT_EQ(Dataset::Type::INT8, dt.getType());
    ASSERT_EQ(-5, dt.getValue<int8_t>(11, 0));
}

TEST_F(DatasetTests, moveConstructFromVectorOfUInt8_t_propertiesAsExpected)
{
    Dataset dt(std::vector<uint8_t>(3, int8_t(33)));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(3, dt.getRows());
    ASSERT_EQ(Dataset::Type::UINT8, dt.getType());
    ASSERT_EQ(33, dt.getValue<uint8_t>(1, 0));
}

TEST_F(DatasetTests, moveConstructFromVectorOfInt16_t_propertiesAsExpected)
{
    Dataset dt(std::vector<int16_t>(10, int16_t(-500)));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(10, dt.getRows());
    ASSERT_EQ(Dataset::Type::INT16, dt.getType());
    ASSERT_EQ(-500, dt.getValue<int16_t>(5, 0));
}

TEST_F(DatasetTests, moveConstructFromVectorOfUInt16_t_propertiesAsExpected)
{
    Dataset dt(std::vector<uint16_t>(4, int16_t(300)));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(4, dt.getRows());
    ASSERT_EQ(Dataset::Type::UINT16, dt.getType());
    ASSERT_EQ(300, dt.getValue<uint16_t>(1, 0));
}

TEST_F(DatasetTests, moveConstructFromVectorOfInt32_t_propertiesAsExpected)
{
    Dataset dt(std::vector<int32_t>(5, int32_t(-2000000000)));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(5, dt.getRows());
    ASSERT_EQ(Dataset::Type::INT32, dt.getType());
    ASSERT_EQ(-2000000000, dt.getValue<int32_t>(2, 0));
}

TEST_F(DatasetTests, moveConstructFromVectorOfUInt32_t_propertiesAsExpected)
{
    Dataset dt(std::vector<uint32_t>(1, uint32_t(4294967295)));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(1, dt.getRows());
    ASSERT_EQ(Dataset::Type::UINT32, dt.getType());
    ASSERT_EQ(4294967295, dt.getValue<uint32_t>(0, 0));
}

TEST_F(DatasetTests, moveConstructFromVectorOfInt64_t_propertiesAsExpected)
{
    Dataset dt(std::vector<int64_t>(3, int64_t(-9223372036854775807)));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(3, dt.getRows());
    ASSERT_EQ(Dataset::Type::INT64, dt.getType());
    ASSERT_EQ(-9223372036854775807, dt.getValue<int64_t>(2, 0));
}

TEST_F(DatasetTests, moveConstructFromVectorOfUInt64_t_propertiesAsExpected)
{
    Dataset dt(std::vector<uint64_t>(1, uint64_t(18446744073709551615U)));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(1, dt.getRows());
    ASSERT_EQ(Dataset::Type::UINT64, dt.getType());
    ASSERT_EQ(18446744073709551615U, dt.getValue<uint64_t>(0, 0));
}

TEST_F(DatasetTests, moveConstructFromVectorOfFloat_propertiesAsExpected)
{
    Dataset dt(std::vector<float>(3, -42.42f));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(3, dt.getRows());
    ASSERT_EQ(Dataset::Type::FLOAT32, dt.getType());
    ASSERT_EQ(-42.42f, dt.getValue<float>(2, 0));
}

TEST_F(DatasetTests, moveConstructFromVectorOfDouble_propertiesAsExpected)
{
    Dataset dt(std::vector<double>(3, -42.42));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(3, dt.getRows());
    ASSERT_EQ(Dataset::Type::FLOAT64, dt.getType());
    ASSERT_EQ(-42.42, dt.getValue<double>(2, 0));
}

TEST_F(DatasetTests, constructFromInt8_propertiesAsExpected)
{
    Dataset dt(int8_t(-39));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(1, dt.getRows());
    ASSERT_EQ(Dataset::Type::INT8, dt.getType());
    ASSERT_EQ(-39, dt.getValue<int8_t>(0, 0));
}

TEST_F(DatasetTests, constructFromUInt8_propertiesAsExpected)
{
    Dataset dt(uint8_t(39));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(1, dt.getRows());
    ASSERT_EQ(Dataset::Type::UINT8, dt.getType());
    ASSERT_EQ(39, dt.getValue<uint8_t>(0, 0));
}

TEST_F(DatasetTests, constructFromInt16_propertiesAsExpected)
{
    Dataset dt(int16_t(-1000));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(1, dt.getRows());
    ASSERT_EQ(Dataset::Type::INT16, dt.getType());
    ASSERT_EQ(-1000, dt.getValue<int16_t>(0, 0));
}

TEST_F(DatasetTests, constructFromUInt16_propertiesAsExpected)
{
    Dataset dt(uint16_t(9999));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(1, dt.getRows());
    ASSERT_EQ(Dataset::Type::UINT16, dt.getType());
    ASSERT_EQ(9999, dt.getValue<uint16_t>(0, 0));
}

TEST_F(DatasetTests, constructFromInt32_propertiesAsExpected)
{
    Dataset dt(int32_t(-2147483647));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(1, dt.getRows());
    ASSERT_EQ(Dataset::Type::INT32, dt.getType());
    ASSERT_EQ(-2147483647, dt.getValue<int32_t>(0, 0));
}

TEST_F(DatasetTests, constructFromUInt32_propertiesAsExpected)
{
    Dataset dt(uint32_t(4294967295));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(1, dt.getRows());
    ASSERT_EQ(Dataset::Type::UINT32, dt.getType());
    ASSERT_EQ(4294967295, dt.getValue<uint32_t>(0, 0));
}

TEST_F(DatasetTests, constructFromInt64_propertiesAsExpected)
{
    Dataset dt(int64_t(-9223372036854775807L));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(1, dt.getRows());
    ASSERT_EQ(Dataset::Type::INT64, dt.getType());
    ASSERT_EQ(-9223372036854775807L, dt.getValue<int64_t>(0, 0));
}

TEST_F(DatasetTests, constructFromUInt64_propertiesAsExpected)
{
    Dataset dt(uint64_t(18446744073709551615UL));
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(1, dt.getRows());
    ASSERT_EQ(Dataset::Type::UINT64, dt.getType());
    ASSERT_EQ(18446744073709551615UL, dt.getValue<uint64_t>(0, 0));
}

TEST_F(DatasetTests, constructFromFloat_propertiesAsExpected)
{
    Dataset dt(42.3f);
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(1, dt.getRows());
    ASSERT_EQ(Dataset::Type::FLOAT32, dt.getType());
    ASSERT_EQ(42.3f, dt.getValue<float>(0, 0));
}

TEST_F(DatasetTests, constructFromDouble_propertiesAsExpected)
{
    Dataset dt(42.317);
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(1, dt.getRows());
    ASSERT_EQ(Dataset::Type::FLOAT64, dt.getType());
    ASSERT_EQ(42.317, dt.getValue<double>(0, 0));
}

TEST_F(DatasetTests, constructFromVectorOfComplexDouble_propertiesAsExpected)
{
    std::vector<std::complex<double>> v = {{3.4, -2.0}, {19.2, 0.1}};
    Dataset dt(v);
    ASSERT_EQ(2, dt.getCols());
    ASSERT_EQ(2, dt.getRows());
    ASSERT_EQ(Dataset::Type::FLOAT64, dt.getType());
    ASSERT_EQ(3.4, dt.getValue<double>(0, 0));
    ASSERT_EQ(-2.0, dt.getValue<double>(0, 1));
    ASSERT_EQ(19.2, dt.getValue<double>(1, 0));
    ASSERT_EQ(0.1, dt.getValue<double>(1, 1));
}

TEST_F(DatasetTests, constructFromRawMemory_propertiesAsExpeced)
{
    std::string buffer("bla\n");
    Dataset dt(buffer.data(), 5);
    ASSERT_EQ(1, dt.getCols());
    ASSERT_EQ(5, dt.getRows());
    ASSERT_EQ(Dataset::Type::UINT8, dt.getType());
    ASSERT_EQ('b', dt.getValue<uint8_t>(0, 0));
    ASSERT_EQ('l', dt.getValue<uint8_t>(1, 0));
    ASSERT_EQ('a', dt.getValue<uint8_t>(2, 0));
    ASSERT_EQ('\n', dt.getValue<uint8_t>(3, 0));
    ASSERT_EQ(0, dt.getValue<uint8_t>(4, 0));
}

TEST_F(DatasetTests, getType_forEmptyVectors_worksAsExpected)
{
    Dataset ds;
    ASSERT_EQ(Dataset::Type::EMPTY, ds.getType());
    Dataset emptyReal((std::vector<real_t>()));
    ASSERT_EQ(Dataset::Type::EMPTY, emptyReal.getType());
    Dataset emptyLabel((std::vector<label_t>()));
    ASSERT_EQ(Dataset::Type::EMPTY, emptyLabel.getType());
}

TEST_F(DatasetTests, getRows_valueCorrect)
{
    Dataset dt(std::vector<real_t>(6, 1.0), 2);
    ASSERT_EQ(3, dt.getRows());
}

TEST_F(DatasetTests, getCols_valueCorrect)
{
    Dataset dt(std::vector<real_t>(8, 2.0), 4);
    ASSERT_EQ(4, dt.getCols());

    Dataset empty;
    ASSERT_EQ(1, empty.getCols());
}

TEST_F(DatasetTests, getValue_wideningConversionFromUint8_valueCorrect)
{
    Dataset dt(uint8_t(42));
    ASSERT_EQ(42, dt.getValue<int8_t>(0, 0));
    ASSERT_EQ(42, dt.getValue<int16_t>(0, 0));
    ASSERT_EQ(42u, dt.getValue<uint16_t>(0, 0));
    ASSERT_EQ(42, dt.getValue<int32_t>(0, 0));
    ASSERT_EQ(42u, dt.getValue<uint32_t>(0, 0));
    ASSERT_EQ(42, dt.getValue<int64_t>(0, 0));
    ASSERT_EQ(42u, dt.getValue<uint64_t>(0, 0));
    ASSERT_EQ(42.0f, dt.getValue<float>(0, 0));
    ASSERT_EQ(42.0, dt.getValue<double>(0, 0));
}

TEST_F(DatasetTests, getValue_narrowingConversionFromUint64_valueCorrect)
{
    Dataset dt(uint64_t(42));
    ASSERT_EQ(42, dt.getValue<int8_t>(0, 0));
    ASSERT_EQ(42u, dt.getValue<uint8_t>(0, 0));
    ASSERT_EQ(42, dt.getValue<int16_t>(0, 0));
    ASSERT_EQ(42u, dt.getValue<uint16_t>(0, 0));
    ASSERT_EQ(42, dt.getValue<int32_t>(0, 0));
    ASSERT_EQ(42u, dt.getValue<uint32_t>(0, 0));
    ASSERT_EQ(42, dt.getValue<int64_t>(0, 0));
    ASSERT_EQ(42u, dt.getValue<uint64_t>(0, 0));
    ASSERT_EQ(42.0f, dt.getValue<float>(0, 0));
    ASSERT_EQ(42.0, dt.getValue<double>(0, 0));
}

TEST_F(DatasetTests, getValue_narrowingConversionFromUint64_expectedLossOfPrecision)
{
    uint64_t monster = 18446744073709551615UL;
    Dataset dt(monster);
    ASSERT_EQ(int8_t(monster), dt.getValue<int8_t>(0, 0));
    ASSERT_EQ(uint8_t(monster), dt.getValue<uint8_t>(0, 0));
    ASSERT_EQ(int16_t(monster), dt.getValue<int16_t>(0, 0));
    ASSERT_EQ(uint16_t(monster), dt.getValue<uint16_t>(0, 0));
    ASSERT_EQ(int32_t(monster), dt.getValue<int32_t>(0, 0));
    ASSERT_EQ(uint32_t(monster), dt.getValue<uint32_t>(0, 0));
    ASSERT_EQ(int64_t(monster), dt.getValue<int64_t>(0, 0));
}

TEST_F(DatasetTests, getValues_emptyDataset_returnsEmptyVectorOfDesiredType)
{
    Dataset ds;
    ASSERT_EQ(std::vector<real_t>(), ds.getValues<real_t>(0));
    ASSERT_EQ(std::vector<label_t>(), ds.getValues<label_t>(0));
}

TEST_F(DatasetTests, getValues_noSizeExpected_valuesCorrect)
{
    Dataset dt(std::vector<label_t>({4, 3, -2, 1, -7, 42}), 3);
    ASSERT_EQ(std::vector<label_t>({4, 3, -2, 1, -7, 42}), dt.getValues<label_t>());
}

TEST_F(DatasetTests, getValues_sizeExpected_valuesCorrect)
{
    Dataset dt(std::vector<label_t>({4, 3, -2, 1, -7, 42}), 3);
    ASSERT_EQ(std::vector<label_t>({4, 3, -2, 1, -7, 42}), dt.getValues<label_t>(6));
}

TEST_F(DatasetTests, getPointerToValues_noSizeExpected_valuesCorrect)
{
    Dataset dt(std::vector<real_t>({4.0, 3.0, -2.0, 1.0, -7.5, 42.42}), 3);
    auto v = dt.getPointerToValues<real_t>();
    ASSERT_EQ(std::vector<real_t>({4.0, 3.0, -2.0, 1.0, -7.5, 42.42}), *v);
}

TEST_F(DatasetTests, getPointerToValues_sizeExpected_valuesCorrect)
{
    Dataset dt(std::vector<real_t>({4.0, 3.0, -2.0, 1.0, -7.5, 42.42}), 3);
    auto v = dt.getPointerToValues<real_t>(6);
    ASSERT_EQ(std::vector<real_t>({4.0, 3.0, -2.0, 1.0, -7.5, 42.42}), *v);
}

TEST_F(DatasetTests, getUseCount_valueCorrect)
{
    Dataset ds(std::vector<real_t>({4.0, 3.0, -2.0, 1.0, -7.5, 42.42}), 3);
    ASSERT_EQ(ds.getUseCount(), 1);

    {
        Dataset ds2 = ds;
        ASSERT_EQ(ds.getUseCount(), 2);
        ASSERT_EQ(ds2.getUseCount(), 2);
    }
    ASSERT_EQ(ds.getUseCount(), 1);
}

TEST_F(DatasetTests, getAllocatedSize_valueCorrect)
{
    ASSERT_EQ(Dataset().getAllocatedSize(), 0u);
    ASSERT_EQ(Dataset(std::vector<int8_t>(42, 0)).getAllocatedSize(), 42u * sizeof(int8_t));
    ASSERT_EQ(Dataset(std::vector<uint8_t>(42, 0)).getAllocatedSize(), 42u * sizeof(uint8_t));
    ASSERT_EQ(Dataset(std::vector<int16_t>(42, 0)).getAllocatedSize(), 42u * sizeof(int16_t));
    ASSERT_EQ(Dataset(std::vector<uint16_t>(42, 0)).getAllocatedSize(), 42u * sizeof(uint16_t));
    ASSERT_EQ(Dataset(std::vector<int32_t>(42, 0)).getAllocatedSize(), 42u * sizeof(int32_t));
    ASSERT_EQ(Dataset(std::vector<uint32_t>(42, 0)).getAllocatedSize(), 42u * sizeof(uint32_t));
    ASSERT_EQ(Dataset(std::vector<int64_t>(42, 0)).getAllocatedSize(), 42u * sizeof(int64_t));
    ASSERT_EQ(Dataset(std::vector<uint64_t>(42, 0)).getAllocatedSize(), 42u * sizeof(uint64_t));
    ASSERT_EQ(Dataset(std::vector<float>(42, 0)).getAllocatedSize(), 42u * sizeof(float));
    ASSERT_EQ(Dataset(std::vector<double>(42, 0)).getAllocatedSize(), 42u * sizeof(double));
}

TEST_F(DatasetTests, equality_differentTypes_returnsFalse)
{
    Dataset ds1(std::vector<uint8_t>{2, 6});
    Dataset ds2(std::vector<int8_t>{2, 6});
    ASSERT_NE(ds1, ds2);
}

TEST_F(DatasetTests, equality_differentCols_returnsFalse)
{
    Dataset ds1(std::vector<uint8_t>{2, 6});
    Dataset ds2(std::vector<uint8_t>{2, 6}, 2);
    ASSERT_NE(ds1, ds2);
}

TEST_F(DatasetTests, equality_differentSize_returnsFalse)
{
    Dataset ds1(std::vector<uint8_t>{2, 6});
    Dataset ds2(std::vector<uint8_t>{2, 6, 8});
    ASSERT_NE(ds1, ds2);
}

TEST_F(DatasetTests, equality_identicalSharedPointer_returnsTrue)
{
    Dataset ds1(std::vector<uint8_t>{2, 6});
    Dataset ds2(ds1);
    ASSERT_EQ(ds1, ds2);
}

TEST_F(DatasetTests, equality_identicalValuesContentForEMPTY_returnsTrue)
{
    Dataset ds1;
    Dataset ds2;
    ASSERT_EQ(ds1, ds2);
}

TEST_F(DatasetTests, equality_identicalValuesContentForINT8_returnsTrue)
{
    ASSERT_EQ(Dataset(std::vector<int8_t>{2, 6}), Dataset(std::vector<int8_t>{2, 6}));
}

TEST_F(DatasetTests, equality_identicalValuesContentForUINT8_returnsTrue)
{
    ASSERT_EQ(Dataset(std::vector<uint8_t>{2, 6}), Dataset(std::vector<uint8_t>{2, 6}));
}

TEST_F(DatasetTests, equality_differentValuesContentForINT16_returnsFalse)
{
    ASSERT_NE(Dataset(std::vector<int16_t>{-2, 6}), Dataset(std::vector<int16_t>{2, 6}));
}

TEST_F(DatasetTests, equality_differentValuesContentForUINT16_returnsFalse)
{
    ASSERT_NE(Dataset(std::vector<uint16_t>{2, 700}), Dataset(std::vector<uint16_t>{2, 6}));
}

TEST_F(DatasetTests, equality_identicalValuesContentForINT32_returnsTrue)
{
    ASSERT_EQ(Dataset(std::vector<int32_t>{2, 6}), Dataset(std::vector<int32_t>{2, 6}));
}

TEST_F(DatasetTests, equality_identicalValuesContentForUINT32_returnsTrue)
{
    ASSERT_EQ(Dataset(std::vector<uint32_t>{2, 6}), Dataset(std::vector<uint32_t>{2, 6}));
}

TEST_F(DatasetTests, equality_differentValuesContentForINT64_returnsFalse)
{
    ASSERT_NE(Dataset(std::vector<int64_t>{-2, 6}), Dataset(std::vector<int64_t>{2, 6}));
}

TEST_F(DatasetTests, equality_differentValuesContentForUINT64_returnsFalse)
{
    ASSERT_NE(Dataset(std::vector<uint64_t>{2, 700}), Dataset(std::vector<uint64_t>{2, 6}));
}

TEST_F(DatasetTests, equality_identicalValuesContentForFLOAT32_returnsTrue)
{
    ASSERT_EQ(Dataset(std::vector<float>{2.3f, 6.4f}), Dataset(std::vector<float>{2.3f, 6.4f}));
}

TEST_F(DatasetTests, equality_identicalValuesContentForFLOAT64_returnsTrue)
{
    ASSERT_EQ(Dataset(std::vector<double>{2.3, 6.4}), Dataset(std::vector<double>{2.3, 6.4}));
}
