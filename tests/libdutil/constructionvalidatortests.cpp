#include "libdutil/constructiondata.h"
#include "libdutil/constructionvalidator.h"
#include "libdutil/namedclass.h"
#include "libdutil/projectware.h"
#include "libdutil/settingrule.h"
#include "libdutil/warelistrule.h"
#include "tests/testbase.h"

using namespace DUTIL;

namespace {
class ConstructionValidatorTests : public TestBase
{};
class Tiger : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::Tiger)
{
public:
    D_NAMED_STRING(Name)
    D_NAMED_PARAMETER(Age, label_t)

    static ConstructionValidator const &getConstructionValidator()
    {
        using SR = SettingRule;
        // clang-format off
        static ConstructionValidator cv({
            [](){
              return SR::forNamedParameter<Tiger::Name>(SR::Usage::MANDATORY_NO_DEFAULT, "The unique name of this tiger.");
            }(),
            [](){
              return SR::forNamedParameter<Tiger::Age>(SR::Usage::MANDATORY_NO_DEFAULT, "The current age of this tiger.");
            }()
            },
            {}, // empty warelist rules map
            ProjectWare::getConstructionValidator());
        // clang-format on
        return cv;
    }

    explicit Tiger(ConstructionData const &cd) :
        name_(getConstructionValidator().validateNamedParameter<Name>(cd)),
        age_(getConstructionValidator().validateNamedParameter<Age>(cd))
    {}

    Name getName() const
    {
        return name_;
    }

private:
    const Name name_;
    Age age_;
};

class Elephant : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::Elephant)
{
    D_NAMED_STRING(Name)
    D_NAMED_PARAMETER(Age, label_t)

    static ConstructionValidator const &getConstructionValidator()
    {
        using SR = SettingRule;
        // clang-format off
        static ConstructionValidator cv({
            [](){
              return SR::forNamedParameter<Elephant::Name>(SR::Usage::MANDATORY_NO_DEFAULT, "The unique name of this elephant.");
            }(),
            [](){
              return SR::forNamedParameter<Elephant::Age>(SR::Usage::MANDATORY_NO_DEFAULT, "The current age of this elephant.");
            }()
            },
            {}, // empty warelist rules map
            ProjectWare::getConstructionValidator());
        // clang-format on
        return cv;
    }

    explicit Elephant(ConstructionData const &cd) :
        name_(getConstructionValidator().validateNamedParameter<Name>(cd)),
        age_(getConstructionValidator().validateNamedParameter<Age>(cd))
    {}

private:
    const Name name_;
    Age age_;
};

class Zoo : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::Zoo)
{
public:
    using CatMap = std::map<std::string, std::unique_ptr<Tiger>>;
    D_NAMED_STRING(Name)

    D_NAMED_LABEL(Min_Visitors)
    D_NAMED_LABEL(Max_Visitors)

    D_NAMED_ENUM(Cat, TIGER, LION, JAGUAR, LUX)

    // Define the list of tigers living in the zoo
    D_NAMED_REFERENCE(CatList, Tiger)

    static DUTIL::ConstructionValidator const &getConstructionValidator()
    {
        using SR = SettingRule;
        using WR = WarelistRule;
        // clang-format off
        static const ConstructionValidator cv({
            []() {
                SR sr = SR::forNamedParameter<Name>(SR::Usage::MANDATORY_NO_DEFAULT, "The name of the new zoo.");
                sr.listOfPossibleValues = {"Wilhelma","Hellabrunn","Zoologische Garten Berlin"};
                return sr;
            } (),
            []() {
                SR sr = SR::forNamedEnum<Cat>(SR::Usage::MANDATORY_WITH_DEFAULT, "The type of wildcat living in the zoo.");
                sr.defaultValue = Variant(Cat::LION);
                return sr;
            }(),
            []() {
                SR sr = SR::forNamedParameter<Min_Visitors>(SR::Usage::MANDATORY_WITH_DEFAULT, "The minimal number of visitors needed to open the zoo.");
                sr.minimalValue = Variant(10);
                sr.maximalValue = Variant(50);
                sr.defaultValue = Variant(20);
                return sr;
            }(),
            []() {
                SR sr = SR::forNamedParameter<Max_Visitors>(SR::Usage::MANDATORY_WITH_DEFAULT, "The maximal number of visitors before the zoo closes.");
                sr.maximalValue = Variant(10000);
                sr.minimalValue = Variant(1000);
                sr.defaultValue = Variant(8000);
                return sr;
            }()
        }, // end list of setting rules
        { [](){
            WR wr = WR::forSubobject<CatList>("A subobject for building a Tiger object.");
            wr.usage = WR::Usage::OPTIONAL;
            return wr;}()
        }, // end list of warelist rules
        ProjectWare::getConstructionValidator());
        // clang-format on
        return cv;
    }

    explicit Zoo(DUTIL::ConstructionData const &cd) :
        cat_(getConstructionValidator().validateNamedEnum<Cat>(cd)),
        name_(getConstructionValidator().validateNamedParameter<Name>(cd)),
        min_(getConstructionValidator().validateNamedParameter<Min_Visitors>(cd)),
        max_(getConstructionValidator().validateNamedParameter<Max_Visitors>(cd))
    {
        auto cat = getConstructionValidator().buildSubObject<CatList>(cd);
        if (cat) {
            catmap_.emplace(cat->getName().value(), std::move(cat));
        }
    }

    Cat getCatType() const
    {
        return cat_;
    }

    bool findTiger(std::string name) const
    {
        if (catmap_.find(name) != catmap_.cend())
            return true;
        else
            return false;
    }

    Name getName() const
    {
        return name_;
    }

    Min_Visitors getMinVisitors() const
    {
        return min_;
    }

    Max_Visitors getMaxVisitors() const
    {
        return max_;
    }

private:
    CatMap catmap_;
    Cat cat_;
    Name name_;
    Min_Visitors min_;
    Max_Visitors max_;
};

} // namespace

TEST_F(ConstructionValidatorTests, testDefaultConstruction)
{
    ConstructionData cd;
    ConstructionValidator cv = ConstructionValidator();
    ASSERT_TRUE(cv.getListOfSettingRuleKeys().empty());
    ASSERT_TRUE(cv.check(cd).empty());
}

TEST_F(ConstructionValidatorTests, testConstructionWithSettingRulesAndValidation)
{
    {
        ConstructionData cd = ConstructionData().setEnum<Zoo::Cat>(Zoo::Cat::JAGUAR);
        cd.s.setParameter<Zoo::Name>("Hellabrunn");
        auto cv = Zoo::getConstructionValidator();
        auto validationError = cv.check(cd);
        ASSERT_TRUE(validationError.empty());
        Zoo z = Zoo(cd);
        ASSERT_TRUE(z.getCatType() == Zoo::Cat::JAGUAR);
    }
    {
        // zoo name not in list of possible values
        ConstructionData cd = ConstructionData().setEnum<Zoo::Cat>(Zoo::Cat::JAGUAR);
        cd.s.setParameter<Zoo::Name>("Paul's Zoo");
        auto cv = Zoo::getConstructionValidator();
        auto validationError = cv.check(cd);
        ASSERT_FALSE(validationError.empty());
    }

    // test construction using the default value for Cat memeber
    {
        ConstructionData cd = ConstructionData().setParamter<Zoo::Name>("Wilhelma");
        auto cv = Zoo::getConstructionValidator();
        auto validationError = cv.check(cd);
        ASSERT_TRUE(validationError.empty());
        Zoo z = Zoo(cd);
        ASSERT_TRUE(z.getCatType() == Zoo::Cat::LION);
    }
    {
        // min and max visitor numbers within the limits
        Zoo z(ConstructionData()
                  .setEnum<Zoo::Cat>(Zoo::Cat::TIGER)
                  .setParamter<Zoo::Name>("Hellabrunn")
                  .setParamter<Zoo::Min_Visitors>(50)
                  .setParamter<Zoo::Max_Visitors>(1000));
        ASSERT_TRUE(z.getName().value() == "Hellabrunn");
        ASSERT_TRUE(z.getCatType() == Zoo::Cat::TIGER);
        ASSERT_TRUE(z.getMinVisitors() == 50);
        ASSERT_TRUE(z.getMaxVisitors() == 1000);
    }
    {
        // min and max visitor numbers within the limits
        Zoo z(ConstructionData().setEnum<Zoo::Cat>(Zoo::Cat::TIGER).setParamter<Zoo::Name>("Hellabrunn"));
        ASSERT_TRUE(z.getName().value() == "Hellabrunn");
        ASSERT_TRUE(z.getCatType() == Zoo::Cat::TIGER);
        ASSERT_TRUE(z.getMinVisitors() == 20);   // check default value
        ASSERT_TRUE(z.getMaxVisitors() == 8000); // check default value
    }
    {
        // min and max visitor numbers within the limits
        D_EXPECT_THROW(Zoo z(ConstructionData()
                                 .setEnum<Zoo::Cat>(Zoo::Cat::TIGER)
                                 .setParamter<Zoo::Name>("Hellabrunn")
                                 .setParamter<Zoo::Min_Visitors>(5)),
                       "is smaller than the allowed min value");
        D_EXPECT_THROW(Zoo z(ConstructionData()
                                 .setEnum<Zoo::Cat>(Zoo::Cat::TIGER)
                                 .setParamter<Zoo::Name>("Hellabrunn")
                                 .setParamter<Zoo::Min_Visitors>(50000)),
                       "is bigger than the allowed max value");
        D_EXPECT_THROW(Zoo z(ConstructionData()
                                 .setEnum<Zoo::Cat>(Zoo::Cat::TIGER)
                                 .setParamter<Zoo::Name>("Hellabrunn")
                                 .setParamter<Zoo::Max_Visitors>(50000)),
                       "is bigger than the allowed max value");
        D_EXPECT_THROW(Zoo z(ConstructionData()
                                 .setEnum<Zoo::Cat>(Zoo::Cat::TIGER)
                                 .setParamter<Zoo::Name>("Hellabrunn")
                                 .setParamter<Zoo::Max_Visitors>(0)),
                       "is smaller than the allowed min value");
    }
}

TEST_F(ConstructionValidatorTests, testBuildSubobject)
{
    // clang-format off
    Zoo z(ConstructionData()
              .setEnum<Zoo::Cat>(Zoo::Cat::TIGER)
              .setParamter<Zoo::Name>("Hellabrunn")
              .setParamter<Zoo::Min_Visitors>(50)
              .setParamter<Zoo::Max_Visitors>(1000)
              .addSubObject<Zoo::CatList>(
                    ConstructionData()
                        .setParamter<Tiger::Name>("Tiger")
                        .setParamter<Tiger::Age>(7))
          );
    // clang-format on
    auto result = z.findTiger("Tiger");
    ASSERT_TRUE(result);
}
