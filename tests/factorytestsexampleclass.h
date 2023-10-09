#ifndef LIBD_TESTS_FACTORYTESTSEXAMPLECLASS_H
#define LIBD_TESTS_FACTORYTESTSEXAMPLECLASS_H
#include "compoundware.h"
#include "libd/libdutil/concretefactory.h"
#include "libd/libdutil/namedclass.h"
#include "libd/libdutil/namedreference.h"
#include "libd/libdutil/projectware.h"

namespace DUTIL {
class ConstructionValidator;
struct ConstructionData;
} // namespace DUTIL

namespace TESTS {
class CompoundWareA;

class Base : public DUTIL::ProjectWare
{
public:
    D_NAMED_BOOL(BaseFlag);
    D_NAMED_REFERENCE(BaseWareAReference, CompoundWareA);
    D_NAMED_REFERENCE(BaseWareAInstance, CompoundWareA);
    static DUTIL::ConstructionValidator const &getConstructionValidator();
    virtual std::string identifyYourself() const = 0;
    template<class Derived>
    static std::unique_ptr<Base> newInstanceViaFactory(DUTIL::ConstructionData const &cd)
    {
        return std::make_unique<Derived>(cd);
    }
};

} // namespace TESTS
D_DECLARE_FACTORYINTERFACE(::TESTS::Base);

#define D_DECLARE_BASEINTERFACE(REGISTERED_CLASS) \
    static const DUTIL::ConcreteFactory<REGISTERED_CLASS, TESTS::Base, DUTIL::ProjectWare> factory;

#define D_DEFINE_BASEINTERFACE(REGISTERED_CLASS) \
    const DUTIL::ConcreteFactory<REGISTERED_CLASS, TESTS::Base, DUTIL::ProjectWare> REGISTERED_CLASS::factory;

namespace TESTS {

class DerivedA : public Base, public D_NAMED_CLASS(::DerivedA)
{
public:
    D_DECLARE_BASEINTERFACE(DerivedA)
    D_NAMED_LABEL(DerivedAInt);
    D_NAMED_REAL(Value);

    static DUTIL::ConstructionValidator const &getConstructionValidator();
    explicit DerivedA(DUTIL::ConstructionData const &cd);
    virtual std::string identifyYourself() const override;
    DerivedAInt id;
};

class DerivedB : public Base, public D_NAMED_CLASS(::DerivedB)
{
public:
    D_DECLARE_BASEINTERFACE(DerivedB)
    D_NAMED_LABEL(DerivedBInt);
    D_NAMED_REAL(Value);

    static DUTIL::ConstructionValidator const &getConstructionValidator();
    void deconstruct(DUTIL::ConstructionData &cd) &&;
    explicit DerivedB(DUTIL::ConstructionData const &cd);
    virtual std::string identifyYourself() const override;
    DerivedBInt id;
};

class CompoundDerived : public Base, public D_NAMED_CLASS(::CompoundDerived)
{
public:
    D_DECLARE_BASEINTERFACE(CompoundDerived)
    D_NAMED_REFERENCE(CompoundWareAInstance, CompoundWareA);

    static DUTIL::ConstructionValidator const &getConstructionValidator();
    //void deconstruct(DUTIL::ConstructionData & cd) &&;
    explicit CompoundDerived(DUTIL::ConstructionData const &cd);
    virtual std::string identifyYourself() const override;
    CompoundWareA wareA;
};

class TemporaryNewConcreteType : public Base, public D_NAMED_CLASS(::TemporaryNewConcreteType)
{
public:
    D_DECLARE_BASEINTERFACE(TemporaryNewConcreteType)
    D_NAMED_REAL(TemporaryReal);

    static DUTIL::ConstructionValidator const &getConstructionValidator();
    virtual std::string identifyYourself() const override;
    explicit TemporaryNewConcreteType(DUTIL::ConstructionData const &);
};
} // namespace TESTS
#endif // LIBD_TESTS_FACTORYTESTSEXAMPLECLASS_H
