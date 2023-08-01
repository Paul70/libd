#include "libdutil/namedclass.h"
#include "libdutil/namedreference.h"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class NamedReferenceTests : public TestBase
{};

class Reference : public D_NAMED_CLASS(::DUTIL::Reference)
{
public:
    bool getTrue() const
    {
        return true;
    }
};

} // namespace

TEST_F(NamedReferenceTests, testDefaultConstruction)
{
    D_NAMED_REFERENCE(Ref, Reference)
    Ref r;
    ASSERT_EQ(r.getId(), "");
}

TEST_F(NamedReferenceTests, testConstructionWithIdAndNullpointer)
{
    D_NAMED_REFERENCE(Ref, Reference)
    Ref r = Ref("MySmartReference");
    ASSERT_EQ(r.getId(), "MySmartReference");
}

TEST_F(NamedReferenceTests, testConstructionWithIdAndPointer)
{
    D_NAMED_REFERENCE(Ref, Reference)
    Ref r = Ref("MySmartReference", std::make_shared<Reference>());
    ASSERT_EQ(r.getId(), "MySmartReference");
    bool result = r->getTrue();
    ASSERT_TRUE(result);
    auto ref = r.ptr();
    ASSERT_TRUE(ref->getTrue());
}

TEST_F(NamedReferenceTests, testConstructionWithVariantAndPointer)
{
    D_NAMED_REFERENCE(Ref, Reference)
    Ref r = Ref(Variant("MySmartReference"), std::make_shared<Reference>());
    ASSERT_EQ(r.getId(), "MySmartReference");
    ASSERT_EQ(static_cast<std::string>(r), "MySmartReference");
    bool result = r->getTrue();
    ASSERT_TRUE(result);
}

TEST_F(NamedReferenceTests, testGetReferenceName)
{
    D_NAMED_REFERENCE(Ref, Reference)
    Ref r;
    ASSERT_EQ(r.getReferenceName(), "Ref");
}

TEST_F(NamedReferenceTests, testFunctions)
{
    D_NAMED_REFERENCE(Ref, Reference)
    Ref r = Ref("MySmartReference", std::make_shared<Reference>());
    bool result = r->getTrue();
    ASSERT_TRUE(result);
    auto ref = r.ptr();
    ASSERT_TRUE(ref->getTrue());
    std::string name = r; // implicit conversion
    ASSERT_EQ(name, r.getId());
    name = static_cast<std::string>(r); // explicit conversion
    ASSERT_EQ(name, r.getId());

    {
        Ref::Pointer p = r; // implicit conversion
        ASSERT_TRUE(p->getTrue());
    }
    {
        Ref::Pointer p = static_cast<Ref::Pointer>(r); // explicit conversion
        ASSERT_TRUE(p->getTrue());
    }
}

TEST_F(NamedReferenceTests, testGetReferredTypeName)
{
    D_NAMED_REFERENCE(Ref, Reference)
    auto typ = Ref::getReferredTypeName();
    ASSERT_EQ(typ, Reference::getClassName());
}
