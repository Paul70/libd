#include "dataset.h"
#include "constructiondata.h"
#include "exception.h"
#include <algorithm>
#include <climits>
#include <cstring>
#include <limits.h> // for CHAR_BIT
#include <limits>

namespace DUTIL {

Dataset::Dataset() :
    cols_(1),
    size_(0),
    values_(std::make_shared<std::vector<int>>())
{}

Dataset::Dataset(char const *buffer, label_t length, label_t nCols) :
    t_(Type::UINT8),
    size_(length),
    values_(std::make_shared<std::vector<uint8_t>>(length))
{
    std::memcpy(std::static_pointer_cast<std::vector<uint8_t>>(values_)->data(), buffer, std::size_t(length));
    setCols(nCols);
}

Dataset::Type Dataset::getType() const
{
    return t_;
}

label_t Dataset::getRows() const
{
    return size_ / cols_;
}

label_t Dataset::getCols() const
{
    return cols_;
}

void Dataset::setCols(label_t cols)
{
    if (cols < 1)
        D_THROW("number of columns must be positive");
    if (size_ % cols)
        D_THROW("array size must be divisible by number of columns");
    if (getType() == Type::EMPTY)
        cols = 1;
    cols_ = cols;
}

label_t Dataset::translateIndex(label_t i, label_t j) const
{
    if (j < 0 || j >= cols_)
        D_THROW(std::string("illegal column index ") + Utility::toString(j) + ", number of columns is currently "
                + Utility::toString(cols_));
    label_t index = i * cols_ + j;
    if (index < 0 || index >= size_)
        D_THROW(std::string("illegal row index ") + Utility::toString(i) + ", number of rows is currently "
                + Utility::toString(size_ / cols_));
    return index;
}

namespace {
void checkSize(label_t expectedSize, label_t actualSize)
{
    if (expectedSize > -1 && expectedSize != actualSize)
        D_THROW(std::string("mismatch between stored data size: data vector has size ") + Utility::toString(actualSize)
                + ", expected " + Utility::toString(expectedSize));
}

template<typename RESULT_TYPE, typename INTERNAL_TYPE>
std::shared_ptr<std::vector<RESULT_TYPE>> convertValues(std::shared_ptr<void> values)
{
    if (std::is_same<RESULT_TYPE, INTERNAL_TYPE>())
        return std::static_pointer_cast<std::vector<RESULT_TYPE>>(values);
    auto actualValues = std::static_pointer_cast<std::vector<INTERNAL_TYPE>>(values);
    size_t size = actualValues->size();
    auto convertedValues = std::make_shared<std::vector<RESULT_TYPE>>(size);
    for (size_t i = 0; i < size; i++)
        (*convertedValues)[i] = static_cast<RESULT_TYPE>((*actualValues)[i]);
    return convertedValues;
}

static_assert(CHAR_BIT == 8, "This code assumes that char is an 8-bit type.");
static_assert(sizeof(float) == 4, "This code assumes that float is a 32-bit type.");
static_assert(sizeof(double) == 8, "This code assumes that double is a 64-bit type.");

template<typename RESULT_TYPE>
std::shared_ptr<std::vector<RESULT_TYPE>> convertValues(Dataset::Type t, std::shared_ptr<void> values)
{
    switch (t) {
    case Dataset::Type::EMPTY:
        return std::make_shared<std::vector<RESULT_TYPE>>();
    case Dataset::Type::INT8:
        return convertValues<RESULT_TYPE, int8_t>(values);
    case Dataset::Type::UINT8:
        return convertValues<RESULT_TYPE, uint8_t>(values);
    case Dataset::Type::INT16:
        return convertValues<RESULT_TYPE, int16_t>(values);
    case Dataset::Type::UINT16:
        return convertValues<RESULT_TYPE, uint16_t>(values);
    case Dataset::Type::INT32:
        return convertValues<RESULT_TYPE, int32_t>(values);
    case Dataset::Type::UINT32:
        return convertValues<RESULT_TYPE, uint32_t>(values);
    case Dataset::Type::INT64:
        return convertValues<RESULT_TYPE, int64_t>(values);
    case Dataset::Type::UINT64:
        return convertValues<RESULT_TYPE, uint64_t>(values);
    case Dataset::Type::FLOAT32:
        return convertValues<RESULT_TYPE, float>(values);
    case Dataset::Type::FLOAT64:
        return convertValues<RESULT_TYPE, double>(values);
    }
    return nullptr;
}
} // end anonymous namespace

std::shared_ptr<std::vector<int8_t>> Dataset::getValuesINT8(label_t expectedSize) const
{
    checkSize(expectedSize, size_);
    return convertValues<int8_t>(t_, values_);
}

std::shared_ptr<std::vector<uint8_t>> Dataset::getValuesUINT8(label_t expectedSize) const
{
    checkSize(expectedSize, size_);
    return convertValues<uint8_t>(t_, values_);
}

std::shared_ptr<std::vector<int16_t>> Dataset::getValuesINT16(label_t expectedSize) const
{
    checkSize(expectedSize, size_);
    return convertValues<int16_t>(t_, values_);
}

std::shared_ptr<std::vector<uint16_t>> Dataset::getValuesUINT16(label_t expectedSize) const
{
    checkSize(expectedSize, size_);
    return convertValues<uint16_t>(t_, values_);
}

std::shared_ptr<std::vector<int32_t>> Dataset::getValuesINT32(label_t expectedSize) const
{
    checkSize(expectedSize, size_);
    return convertValues<int32_t>(t_, values_);
}

std::shared_ptr<std::vector<uint32_t>> Dataset::getValuesUINT32(label_t expectedSize) const
{
    checkSize(expectedSize, size_);
    return convertValues<uint32_t>(t_, values_);
}

std::shared_ptr<std::vector<int64_t>> Dataset::getValuesINT64(label_t expectedSize) const
{
    checkSize(expectedSize, size_);
    return convertValues<int64_t>(t_, values_);
}

std::shared_ptr<std::vector<uint64_t>> Dataset::getValuesUINT64(label_t expectedSize) const
{
    checkSize(expectedSize, size_);
    return convertValues<uint64_t>(t_, values_);
}

#if defined(D_MINGW_MXE) || defined(D_MINGW_NATIVE) || defined(D_MSVC)
std::shared_ptr<std::vector<long>> Dataset::getValuesLONG(label_t expectedSize) const
{
    checkSize(expectedSize, size_);
    return convertValues<long>(t_, values_);
}
#endif

std::shared_ptr<std::vector<float>> Dataset::getValuesFLOAT(label_t expectedSize) const
{
    checkSize(expectedSize, size_);
    return convertValues<float>(t_, values_);
}

std::shared_ptr<std::vector<double>> Dataset::getValuesDOUBLE(label_t expectedSize) const
{
    checkSize(expectedSize, size_);
    return convertValues<double>(t_, values_);
}

template<>
Dataset::Type Dataset::TypeMap<int8_t>()
{
    return Type::INT8;
}
template<>
Dataset::Type Dataset::TypeMap<uint8_t>()
{
    return Type::UINT8;
}
template<>
Dataset::Type Dataset::TypeMap<int16_t>()
{
    return Type::INT16;
}
template<>
Dataset::Type Dataset::TypeMap<uint16_t>()
{
    return Type::UINT16;
}
template<>
Dataset::Type Dataset::TypeMap<int32_t>()
{
    return Type::INT32;
}
template<>
Dataset::Type Dataset::TypeMap<uint32_t>()
{
    return Type::UINT32;
}
template<>
Dataset::Type Dataset::TypeMap<int64_t>()
{
    return Type::INT64;
}
template<>
Dataset::Type Dataset::TypeMap<uint64_t>()
{
    return Type::UINT64;
}
template<>
Dataset::Type Dataset::TypeMap<float>()
{
    return Type::FLOAT32;
}
template<>
Dataset::Type Dataset::TypeMap<double>()
{
    return Type::FLOAT64;
}

#if defined(D_MINGW_MXE) || defined(D_MINGW_NATIVE) || defined(D_MSVC)
template<>
Dataset::Type Dataset::TypeMap<long>()
{
    static_assert(sizeof(long) == 4 || sizeof(long) == 8);
    if constexpr (sizeof(long) == 4)
        return Type::INT32;
    else
        return Type::INT64;
}
#endif

template<>
std::shared_ptr<const std::vector<int8_t>> Dataset::getPointerToValues<int8_t>(label_t expectedSize) const
{
    return getValuesINT8(expectedSize);
}
template<>
std::shared_ptr<const std::vector<uint8_t>> Dataset::getPointerToValues<uint8_t>(label_t expectedSize) const
{
    return getValuesUINT8(expectedSize);
}
template<>
std::shared_ptr<const std::vector<int16_t>> Dataset::getPointerToValues<int16_t>(label_t expectedSize) const
{
    return getValuesINT16(expectedSize);
}
template<>
std::shared_ptr<const std::vector<uint16_t>> Dataset::getPointerToValues<uint16_t>(label_t expectedSize) const
{
    return getValuesUINT16(expectedSize);
}
template<>
std::shared_ptr<const std::vector<int32_t>> Dataset::getPointerToValues<int32_t>(label_t expectedSize) const
{
    return getValuesINT32(expectedSize);
}
template<>
std::shared_ptr<const std::vector<uint32_t>> Dataset::getPointerToValues<uint32_t>(label_t expectedSize) const
{
    return getValuesUINT32(expectedSize);
}
template<>
std::shared_ptr<const std::vector<int64_t>> Dataset::getPointerToValues<int64_t>(label_t expectedSize) const
{
    return getValuesINT64(expectedSize);
}
template<>
std::shared_ptr<const std::vector<uint64_t>> Dataset::getPointerToValues<uint64_t>(label_t expectedSize) const
{
    return getValuesUINT64(expectedSize);
}
template<>
std::shared_ptr<const std::vector<float>> Dataset::getPointerToValues<float>(label_t expectedSize) const
{
    return getValuesFLOAT(expectedSize);
}
template<>
std::shared_ptr<const std::vector<double>> Dataset::getPointerToValues<double>(label_t expectedSize) const
{
    return getValuesDOUBLE(expectedSize);
}
#if defined(D_MINGW_MXE) || defined(D_MINGW_NATIVE) || defined(D_MSVC)
template<>
std::shared_ptr<const std::vector<long>> Dataset::getPointerToValues<long>(label_t expectedSize) const
{
    return getValuesLONG(expectedSize);
}
#endif

label_t Dataset::getUseCount() const
{
    return values_.use_count();
}

size_t Dataset::getAllocatedSize() const
{
    // Since the vector is never reallocated, we expect capacity to equal size.
    // No guarantee for this though.
    switch (t_) {
    case Type::INT8:
        return sizeof(int8_t) * std::static_pointer_cast<std::vector<int8_t>>(values_)->capacity();
    case Type::UINT8:
        return sizeof(uint8_t) * std::static_pointer_cast<std::vector<uint8_t>>(values_)->capacity();
    case Type::INT16:
        return sizeof(int16_t) * std::static_pointer_cast<std::vector<int16_t>>(values_)->capacity();
    case Type::UINT16:
        return sizeof(uint16_t) * std::static_pointer_cast<std::vector<uint16_t>>(values_)->capacity();
    case Type::INT32:
        return sizeof(int32_t) * std::static_pointer_cast<std::vector<int32_t>>(values_)->capacity();
    case Type::UINT32:
        return sizeof(uint32_t) * std::static_pointer_cast<std::vector<uint32_t>>(values_)->capacity();
    case Type::INT64:
        return sizeof(int64_t) * std::static_pointer_cast<std::vector<int64_t>>(values_)->capacity();
    case Type::UINT64:
        return sizeof(uint64_t) * std::static_pointer_cast<std::vector<uint64_t>>(values_)->capacity();
    case Type::FLOAT32:
        return sizeof(float) * std::static_pointer_cast<std::vector<float>>(values_)->capacity();
    case Type::FLOAT64:
        return sizeof(double) * std::static_pointer_cast<std::vector<double>>(values_)->capacity();
    }

    return 0; // for EMPTY
}

bool Dataset::operator==(Dataset const &other) const
{
    if (t_ != other.t_)
        return false;
    if (cols_ != other.cols_)
        return false;
    if (size_ != other.size_)
        return false;

    if (values_ == other.values_)
        return true;

    switch (t_) {
    case Type::EMPTY:
        return true;
    case Type::INT8:
        return *std::static_pointer_cast<std::vector<int8_t>>(values_)
               == *std::static_pointer_cast<std::vector<int8_t>>(other.values_);
    case Type::UINT8:
        return *std::static_pointer_cast<std::vector<uint8_t>>(values_)
               == *std::static_pointer_cast<std::vector<uint8_t>>(other.values_);
    case Type::INT16:
        return *std::static_pointer_cast<std::vector<int16_t>>(values_)
               == *std::static_pointer_cast<std::vector<int16_t>>(other.values_);
    case Type::UINT16:
        return *std::static_pointer_cast<std::vector<uint16_t>>(values_)
               == *std::static_pointer_cast<std::vector<uint16_t>>(other.values_);
    case Type::INT32:
        return *std::static_pointer_cast<std::vector<int32_t>>(values_)
               == *std::static_pointer_cast<std::vector<int32_t>>(other.values_);
    case Type::UINT32:
        return *std::static_pointer_cast<std::vector<uint32_t>>(values_)
               == *std::static_pointer_cast<std::vector<uint32_t>>(other.values_);
    case Type::INT64:
        return *std::static_pointer_cast<std::vector<int64_t>>(values_)
               == *std::static_pointer_cast<std::vector<int64_t>>(other.values_);
    case Type::UINT64:
        return *std::static_pointer_cast<std::vector<uint64_t>>(values_)
               == *std::static_pointer_cast<std::vector<uint64_t>>(other.values_);
    case Type::FLOAT32:
        return *std::static_pointer_cast<std::vector<float>>(values_)
               == *std::static_pointer_cast<std::vector<float>>(other.values_);
    case Type::FLOAT64:
        return *std::static_pointer_cast<std::vector<double>>(values_)
               == *std::static_pointer_cast<std::vector<double>>(other.values_);
    }

    return false;
}

bool Dataset::operator!=(Dataset const &other) const
{
    return !(*this == other);
}

} // namespace DUTIL
