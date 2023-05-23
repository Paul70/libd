#include "variant.h"

using namespace DUTIL;
using namespace DUTIL::VariantDetail;

Variant::Variant() :
    var_(std::monostate()),
    type_(Type::MONOSTATE)
{}

Variant::Type Variant::getType() const
{
    return type_;
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
    return type_ >= Type::INT64 && type_ <= Type::DOUBLE;
}

bool Variant::isCharacter() const
{
    return type_ == Type::CHAR;
}

bool Variant::isBool() const
{
    return type_ == Type::BOOL;
}
