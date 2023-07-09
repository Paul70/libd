#include "variant.h"
#include "exception.h"
#include <tuple>

namespace DUTIL {
using namespace DUTIL::VariantDetail;

Variant::Variant() :
    var_(std::monostate()),
    type_(Type::MONOSTATE)
{}

bool Variant::isNumeric(Type t)
{
    return t >= Type::LABEL && t <= Type::DOUBLE;
}

Variant::Type Variant::getType() const
{
    return type_;
}

bool Variant::isValid() const
{
    return type_ > Type::MONOSTATE;
}

bool Variant::isMonostate() const
{
    return type_ == Type::MONOSTATE;
}

bool Variant::isString() const
{
    return type_ == Type::STRING;
}

bool Variant::isNumeric() const
{
    return type_ >= Type::LABEL && type_ <= Type::DOUBLE;
}

bool Variant::isCharacter() const
{
    return type_ == Type::CHAR;
}

bool Variant::isBool() const
{
    return type_ == Type::BOOL;
}

std::string Variant::toString() const
{
    auto result = getAs<std::string>();
    if (!result.first)
        D_THROW("Variant holding type '" + type_.toString() + "' is not convertible into std::string.");
    return result.second;
}

bool Variant::toBool() const
{
    auto result = getAs<bool>();
    if (!result.first)
        D_THROW("Variant holding type '" + type_.toString() + "' is not convertible into bool.");
    return result.second;
}

label_t Variant::toLabel() const
{
    auto result = getAs<label_t>();
    if (!result.first)
        D_THROW("Variant holding type '" + type_.toString() + "' is not convertible into lable_t alias int.");
    return result.second;
}

real_t Variant::toReal() const
{
    auto result = getAs<real_t>();
    if (!result.first)
        D_THROW("Variant holding type '" + type_.toString() + "' is not convertible into real_t alias double.");
    return result.second;
}

Variant &Variant::convertTo(Type t)
{
    switch (t) {
    case Type::MONOSTATE: {
        DutilVariant v = DutilVariant();
        var_.swap(v);
        break;
    }
    case Type::LABEL: {
        auto result = getAs<label_t>();
        if (result.first)
            var_ = result.second;
        break;
    }
    case Type::INT64: {
        auto result = getAs<std::int64_t>();
        if (result.first)
            var_ = result.second;
        break;
    }
    case Type::UINT64: {
        auto result = getAs<std::uint64_t>();
        if (result.first)
            var_ = result.second;
        break;
    }
    case Type::DOUBLE: {
        auto result = getAs<real_t>();
        if (result.first)
            var_ = result.second;
        break;
    }
    case Type::CHAR: {
        auto result = getAs<char>();
        if (result.first)
            var_ = result.second;
        break;
    }
    case Type::STRING: {
        auto result = getAs<std::string>();
        if (result.first)
            var_ = result.second;
        break;
    }
    }
    type_ = t;
    return *this;
}

bool operator==(Variant const &lhs, Variant const &rhs)
{
    // create std::tie objects for lhs-ref and rhs-ref and use std::tuple's
    // lexicographical operators
    auto lhsType = lhs.getType();
    auto rhsType = rhs.getType();
    return (std::tie(lhsType, lhs.get()) == std::tie(rhsType, rhs.get()));
}

bool operator!=(Variant const &lhs, Variant const &rhs)
{
    return !(lhs == rhs);
}

Variant::DutilVariant const &Variant::get() const
{
    return var_;
}
} // namespace DUTIL
