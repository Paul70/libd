#ifndef LIBD_TESTS_COMPOUNDWARE_H
#define LIBD_TESTS_COMPOUNDWARE_H
#include "libdutil/concretefactory.h"
#include "libdutil/namedclass.h"
#include "libdutil/projectware.h"

namespace TESTS {

class CompoundWareA final : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::CompoundWareA)
{
  public:
  //! Static factory member.
  D_DECLARE_PROJECTWARE(CompoundWareA);

  D_NAMED_LABEL(LabelA);
  LabelA lA;

  static DUTIL::ConstructionValidator const& getConstructionValidator();
  //void deconstruct(DUTIL::ConstructionData & cd) &&;
  explicit CompoundWareA(DUTIL::ConstructionData const& cd);
  //CompoundWareA(CompoundWareA const&) = delete;
  void operator=(CompoundWareA const&) = delete;
  CompoundWareA(CompoundWareA&& other);
};

class CompoundWareB final : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::CompoundWareB)
{
  public:
  //! Static factory member.
  D_DECLARE_PROJECTWARE(CompoundWareB);

  D_NAMED_REFERENCE(WareAPtrRef, CompoundWareA);
  D_NAMED_REAL(RealB);

  //! Shared ware member meaning that it gets initialized with a shared pointer.´
  WareAPtrRef wareARef;
  RealB rB;

  static DUTIL::ConstructionValidator const& getConstructionValidator();
  //void deconstruct(DUTIL::ConstructionData & cd) &&;
  explicit CompoundWareB(DUTIL::ConstructionData const& cd);
};

class CompoundWareBIntermediate : public DUTIL::ProjectWare
{
  public:
  D_NAMED_REFERENCE(WareBInstance, CompoundWareB);
  D_NAMED_REAL(RealBIntermediate);

  //! Shared ware member meaning that it gets initialized with a shared pointer.´
  CompoundWareB wareB;
  RealBIntermediate rB;

  static DUTIL::ConstructionValidator const& getConstructionValidator();
  //void deconstruct(DUTIL::ConstructionData & cd) &&;
  explicit CompoundWareBIntermediate(DUTIL::ConstructionData const& cd);
};

class CompoundWareC final : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::CompoundWareC)
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

  static DUTIL::ConstructionValidator const& getConstructionValidator();
  //void deconstruct(DUTIL::ConstructionData & cd) &&;
  explicit CompoundWareC(DUTIL::ConstructionData const& cd);
};

class CompoundWareD final : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::CompoundWareD)
{
  public:
  //! Static factory member.
  D_DECLARE_PROJECTWARE(CompoundWareD);

  D_NAMED_STRING(NameD);
  NameD name;

  static DUTIL::ConstructionValidator const& getConstructionValidator();
  //void deconstruct(DUTIL::ConstructionData & cd) &&;
  explicit CompoundWareD(DUTIL::ConstructionData const& cd);
  CompoundWareD(CompoundWareD const&) = delete;
  void operator=(CompoundWareD const&) = delete;
  CompoundWareD(CompoundWareD&& other);
};

class CompoundWareE final :
    public CompoundWareBIntermediate,
    public D_NAMED_CLASS(::TESTS::CompoundWareE)
{
  public:
  D_DECLARE_PROJECTWARE(CompoundWareE)

  D_NAMED_STRING(Description)
  D_NAMED_REFERENCE(WareDInstanceList, CompoundWareD)

  static DUTIL::ConstructionValidator const& getConstructionValidator();

  explicit CompoundWareE(DUTIL::ConstructionData const& cd);

  Description descr;
  std::vector<std::unique_ptr<CompoundWareD>> dList;
};

class CompoundWareF final :
    public CompoundWareBIntermediate,
    public D_NAMED_CLASS(::TESTS::CompoundWareF)
{
  public:
  D_DECLARE_PROJECTWARE(CompoundWareF)

  D_NAMED_STRING(Description)

  static DUTIL::ConstructionValidator const& getConstructionValidator();

  explicit CompoundWareF(DUTIL::ConstructionData const& cd);

  Description descr;
};
}  // namespace TESTS

#endif  // LIBD_TESTS_COMPOUNDWARE_H
