#ifndef DUTIL_COMPOUNDWARE_H
#define DUTIL_COMPOUNDWARE_H
#include "libdutil/concretefactory.h"
#include "libdutil/namedclass.h"
#include "libdutil/projectware.h"

namespace TESTS {

class CompoundWareA : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::CompoundWareA)
{
public:
    //! Static factory member.
    D_DECLARE_PROJECTWARE(CompoundWareA);

    D_NAMED_LABEL(LabelA);
    LabelA lA;

    static DUTIL::ConstructionValidator const &getConstructionValidator();
    //void deconstruct(DUTIL::ConstructionData & cd) &&;
    explicit CompoundWareA(DUTIL::ConstructionData const &cd);
    CompoundWareA(CompoundWareA const &) = delete;
    void operator=(CompoundWareA const &) = delete;
    CompoundWareA(CompoundWareA &&other);
};

class CompoundWareB : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::CompoundWareB)
{
public:
    //! Static factory member.
    D_DECLARE_PROJECTWARE(CompoundWareB);

    D_NAMED_REFERENCE(WareAPtrRef, CompoundWareA);
    D_NAMED_REAL(RealB);

    //! Shared ware member meaning that it gets initialized with a shared pointer.´
    WareAPtrRef wareARef;
    RealB rB;

    static DUTIL::ConstructionValidator const &getConstructionValidator();
    //void deconstruct(DUTIL::ConstructionData & cd) &&;
    explicit CompoundWareB(DUTIL::ConstructionData const &cd);
};

class CompoundWareC : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::CompoundWareC)
{
public:
    D_DECLARE_PROJECTWARE(CompoundWareC);
    D_NAMED_REFERENCE(WareAVariableListInstance, CompoundWareA);
    D_NAMED_REFERENCE(WareAVariableListRef, CompoundWareA);
    D_NAMED_REFERENCE(WareAFixedListInstance, CompoundWareA);
    D_NAMED_REFERENCE(WareAFixedListRef, CompoundWareA);
    D_NAMED_REFERENCE(WareBInstance, CompoundWareB);
    D_NAMED_ENUM(ChoiceC, THE_GOOD, THE_BAD, THE_UGLY);

    std::vector<std::unique_ptr<CompoundWareA>> ownWareAVariableList;
    std::vector<WareAVariableListRef> sharedWareAVariableList;
    std::vector<std::unique_ptr<CompoundWareA>> ownWareAFixedList;
    std::vector<WareAFixedListRef> sharedWareAFixedList;
    CompoundWareB wareB;
    ChoiceC cC;

    static DUTIL::ConstructionValidator const &getConstructionValidator();
    //void deconstruct(DUTIL::ConstructionData & cd) &&;
    explicit CompoundWareC(DUTIL::ConstructionData const &cd);
};
} // namespace TESTS

#endif // DUTIL_COMPOUNDWARE_H
