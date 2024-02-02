#ifndef TESTS_CAT_H
#define TESTS_CAT_H
#include "libdutil/basictypes.h"
#include "libdutil/concretefactory.h"
#include "libdutil/namedclass.h"
#include "libdutil/projectware.h"

namespace DUTIL {
struct ConstructionData;
class ConstructionValidator;
}  // namespace DUTIL

namespace TESTS {
class Cat : public DUTIL::ProjectWare
{
  public:
  D_NAMED_ENUM(Type, TIGER, LION, JAGUAR, LUX)

  D_NAMED_STRING(Name)
  D_NAMED_PARAMETER(Age, DUTIL::label_t)

  static DUTIL::ConstructionValidator const& getConstructionValidator();

  explicit Cat(DUTIL::ConstructionData const& cd);

  Cat();

  Cat::Type getType() const;

  Name getName() const;

  Age getAge() const;

  private:
  virtual Cat::Type getTypeImpl() const = 0;

  const Name name_;
  Age age_;
};
}  // namespace TESTS

D_DECLARE_FACTORYINTERFACE(TESTS::Cat)

#define D_DECLARE_CAT(REGISTERED_CLASS) \
  static const DUTIL::ConcreteFactory<REGISTERED_CLASS, ::TESTS::Cat, DUTIL::ProjectWare> factory;

#define D_DEFINE_CAT(REGISTERED_CLASS)                                             \
  const DUTIL::ConcreteFactory<REGISTERED_CLASS, ::TESTS::Cat, DUTIL::ProjectWare> \
      REGISTERED_CLASS::factory;

namespace TESTS {
class Tiger : public TESTS::Cat, public D_NAMED_CLASS(::TESTS::Tiger)
{
  public:
  D_DECLARE_CAT(Tiger)

  D_NAMED_ENUM(Species, AMUR, INDIAN, BENGAL)

  static DUTIL::ConstructionValidator const& getConstructionValidator();

  explicit Tiger(DUTIL::ConstructionData const& cd);

  private:
  virtual Cat::Type getTypeImpl() const override { return type_; }

  const Cat::Type type_ = Cat::Type::TIGER;
  const Species species_;
};

class Jaguar : public Cat, public D_NAMED_CLASS(::Jaguar)
{
  public:
  D_DECLARE_CAT(Jaguar)

  static DUTIL::ConstructionValidator const& getConstructionValidator();

  explicit Jaguar(DUTIL::ConstructionData const& cd);

  private:
  virtual Cat::Type getTypeImpl() const override { return type_; }

  const Cat::Type type_ = Cat::Type::TIGER;
};

class KK : public Cat, public D_NAMED_CLASS(::KK)
{
  public:
  D_DECLARE_CAT(KK)

  //static DUTIL::ConstructionValidator const &getConstructionValidator();

  explicit KK(DUTIL::ConstructionData const& cd) :
      Cat(cd)
  {}

  KK() {}

  private:
  virtual KK::Type getTypeImpl() const override { return type_; }

  const KK::Type type_ = Cat::Type::TIGER;
};

}  // namespace TESTS
#endif  // TESTS_CAT_H
