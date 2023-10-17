#ifndef DUTIL_DATASET_H
#define DUTIL_DATASET_H
#include "namedenum.h"
#include <complex>
#include <memory>
#include <vector>

namespace DUTIL {

/*! \brief Store a matrix of numeric values.
 *
 * The data is stored internally as a flat array. The number of columns
 * can be set by the user to group the values for better understanding.
 * Empty datasets are allowed and have a special type.
 *
 * The types allowed for the data follow what is available in standard C/C++ (header cstdint). Floating point types
 * for the C/C++ types float and double are provided. The code works with 32-bit floats and 64-bit doubles, this is
 * asserted in the implementation. Raw bytes can be stored by representing them as a (1-column) table of uint_8 numbers.
 *
 * Dataset objects act as a bridge between the low-level storage of numerical data and the high-level definition
 * of parameterized physical quantities, such as permittivity as a
 * function of wavelength. Datasets can be read from or written to CSV files or stored in Database objects. Conversion
 * functionality exists in order to interpret the data as vectors of given type.
 */

class Dataset
{
public:
    //! Type of data stored in the dataset.
    D_NAMED_ENUM(Type, EMPTY, INT8, UINT8, INT16, UINT16, INT32, UINT32, INT64, UINT64, FLOAT32, FLOAT64);

    /*! \brief Mapping between Type values and actual C++ types.
     *
     * Note the specializations defined below. Unsupported types should prevent
     * compilation or linking.
     */
    template<typename RESULT_TYPE>
    static Type TypeMap();

    /*! \brief Create an empty Dataset
     *
     * By default, the number of columns is set to 1 and the type to EMPTY.
     */
    Dataset();

    /*! \brief Store a vector of values row-wise in the table.
     *
     * The number of columns is set to 1 by default:
     *
     * v[0]
     * v[1]
     * ...
     *
     * Note that the Dataset type is set to EMPTY if the vector has size 0.
     *
     * nCols must be a positive integer, otherwise trigger fatal error.
     * The size of the raw data array must also be divisible by this number,
     * otherwise trigger fatal error.
     */
    template<typename TYPE>
    explicit Dataset(std::vector<TYPE> values, label_t nCols = 1);

    //! Store a single value in the table.
    template<typename TYPE>
    explicit Dataset(TYPE value);

    /*! \brief Store a vector of complex values using two columns.
     *
     * Column 0 is for the real part, column 1 for the imaginary part.
     *
     * Since we do not explicitly support complex types in this class,
     * the receiving code of such a Dataset has to know how to process
     * the result. The C++ Standard guarantees that a std::complex<T> can be
     * reinterpreted as two scalars of type T laid out in order, thus
     * a simple cast on the raw data to a vector of complex may be
     * all that is needed.
     */
    template<typename TYPE>
    explicit Dataset(std::vector<std::complex<TYPE>> values);

    //! Store "length" raw bytes as uint8_t in the table.
    Dataset(char const *buffer, label_t length, label_t nCols = 1);

    //! \brief Return the data type.
    Type getType() const;

    /*! \brief Return the number of rows in the dataset.
     *
     * Note that getRows() * getCols() is equal to the
     * the size of the data array.
     */
    label_t getRows() const;

    /*! \brief Return the number of columns in the dataset.
     *
     * Note that getRows() * getCols() is equal to the
     * the size of the data array.
     */
    label_t getCols() const;

    /*! \brief Return a copy of data item at row i and column j.
     *
     * Passing illegal values of i and j will trigger an exception.
     * If the desired type does not precisely match the internal type, attempt
     * a conversion.
     */
    template<typename TYPE>
    TYPE getValue(label_t i, label_t j) const;

    /*! \brief Return a copy of the data row-by-row as a flat array.
     *
     * If the desired data type does not match the internal type, attempt
     * an element-wise conversion.
     *
     * If the expected size is set, compare this to the internally stored
     * vector size and trigger an exception if the sizes are not identical.
     *
     * Calling this function on a Dataset of EMPTY type simply returns
     * an empty vector of desired type.
     */
    template<typename TYPE>
    std::vector<TYPE> getValues(label_t expectedSize = -1) const;

    /*! \brief Return a pointer to the data.
     *
     * If the desired data type does not match the internal type, attempt
     * an element-wise conversion, and return a pointer to a new std::vector
     * instance containing the converted values.
     *
     * If the expected size is set, compare this to the internally stored
     * vector size and trigger an exception if the sizes are not identical.
     *
     * Calling this function on a Dataset of EMPTY type returns a pointer to a
     * an empty vector of desired type.
     */
    template<typename TYPE>
    std::shared_ptr<const std::vector<TYPE>> getPointerToValues(label_t expectedSize = -1) const;

    //! \brief Return the use count of the underlying shared_ptr
    label_t getUseCount() const;

    //! \brief Return the allocated size in bytes of the wrapped std::vector
    size_t getAllocatedSize() const;

    //! Compare two settings objects for equality.
    bool operator==(Dataset const &other) const;
    bool operator!=(Dataset const &other) const;

private:
    Type t_;
    label_t cols_;
    label_t size_;
    std::shared_ptr<void> values_;

    std::shared_ptr<std::vector<int8_t>> getValuesINT8(label_t expectedSize) const;
    std::shared_ptr<std::vector<uint8_t>> getValuesUINT8(label_t expectedSize) const;
    std::shared_ptr<std::vector<int16_t>> getValuesINT16(label_t expectedSize) const;
    std::shared_ptr<std::vector<uint16_t>> getValuesUINT16(label_t expectedSize) const;
    std::shared_ptr<std::vector<int32_t>> getValuesINT32(label_t expectedSize) const;
    std::shared_ptr<std::vector<uint32_t>> getValuesUINT32(label_t expectedSize) const;
    std::shared_ptr<std::vector<int64_t>> getValuesINT64(label_t expectedSize) const;
    std::shared_ptr<std::vector<uint64_t>> getValuesUINT64(label_t expectedSize) const;
    std::shared_ptr<std::vector<long>> getValuesLONG(label_t expectedSize) const;
    std::shared_ptr<std::vector<float>> getValuesFLOAT(label_t expectedSize) const;
    std::shared_ptr<std::vector<double>> getValuesDOUBLE(label_t expectedSize) const;
    label_t translateIndex(label_t i, label_t j) const;
    void setCols(label_t cols);
};

template<>
Dataset::Type Dataset::TypeMap<int8_t>();
template<>
Dataset::Type Dataset::TypeMap<uint8_t>();
template<>
Dataset::Type Dataset::TypeMap<int16_t>();
template<>
Dataset::Type Dataset::TypeMap<uint16_t>();
template<>
Dataset::Type Dataset::TypeMap<int32_t>();
template<>
Dataset::Type Dataset::TypeMap<uint32_t>();
template<>
Dataset::Type Dataset::TypeMap<int64_t>();
template<>
Dataset::Type Dataset::TypeMap<uint64_t>();
template<>
Dataset::Type Dataset::TypeMap<long>();
template<>
Dataset::Type Dataset::TypeMap<float>();
template<>
Dataset::Type Dataset::TypeMap<double>();

template<typename TYPE>
Dataset::Dataset(std::vector<TYPE> values, label_t nCols) :
    t_(values.size() ? Dataset::TypeMap<TYPE>() : Type::EMPTY),
    size_(d_size(values)),
    values_(std::make_shared<std::vector<TYPE>>(values))
{
    setCols(nCols);
}

template<typename TYPE>
Dataset::Dataset(TYPE value) :
    t_(Dataset::TypeMap<TYPE>()),
    cols_(1),
    size_(1),
    values_(std::make_shared<std::vector<TYPE>>(1, value))
{}

template<typename TYPE>
Dataset::Dataset(std::vector<std::complex<TYPE>> values) :
    t_(values.size() ? Dataset::TypeMap<TYPE>() : Type::EMPTY),
    size_(2 * d_size(values))
{
    std::vector<TYPE> newValues;
    newValues.reserve(2u * values.size());
    for (auto const &v : values) {
        newValues.push_back(v.real());
        newValues.push_back(v.imag());
    }
    values_ = std::make_shared<std::vector<TYPE>>(std::move(newValues));
    setCols(2);
}

template<>
std::shared_ptr<const std::vector<int8_t>> Dataset::getPointerToValues<int8_t>(label_t expectedSize) const;
template<>
std::shared_ptr<const std::vector<uint8_t>> Dataset::getPointerToValues<uint8_t>(label_t expectedSize) const;
template<>
std::shared_ptr<const std::vector<int16_t>> Dataset::getPointerToValues<int16_t>(label_t expectedSize) const;
template<>
std::shared_ptr<const std::vector<uint16_t>> Dataset::getPointerToValues<uint16_t>(label_t expectedSize) const;
template<>
std::shared_ptr<const std::vector<int32_t>> Dataset::getPointerToValues<int32_t>(label_t expectedSize) const;
template<>
std::shared_ptr<const std::vector<uint32_t>> Dataset::getPointerToValues<uint32_t>(label_t expectedSize) const;
template<>
std::shared_ptr<const std::vector<int64_t>> Dataset::getPointerToValues<int64_t>(label_t expectedSize) const;
template<>
std::shared_ptr<const std::vector<uint64_t>> Dataset::getPointerToValues<uint64_t>(label_t expectedSize) const;
template<>
std::shared_ptr<const std::vector<long>> Dataset::getPointerToValues<long>(label_t expectedSize) const;
template<>
std::shared_ptr<const std::vector<float>> Dataset::getPointerToValues<float>(label_t expectedSize) const;
template<>
std::shared_ptr<const std::vector<double>> Dataset::getPointerToValues<double>(label_t expectedSize) const;

template<typename TYPE>
std::vector<TYPE> Dataset::getValues(label_t expectedSize) const
{
    auto ptr = getPointerToValues<TYPE>(expectedSize);
    return *ptr;
}

template<typename TYPE>
TYPE Dataset::getValue(label_t i, label_t j) const
{
    auto ptr = getPointerToValues<TYPE>(-1);
    return ptr->operator[](translateIndex(i, j));
}

} // namespace DUTIL
#endif // DUTIL_DATASET_H
