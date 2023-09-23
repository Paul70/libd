#include "libdutil/concretefactory.h"
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

class Cat : public DUTIL::ProjectWare
{
public:
    D_NAMED_ENUM(Type, TIGER, LION, JAGUAR, LUX)

    D_NAMED_STRING(Name)
    D_NAMED_PARAMETER(Age, label_t)

    static ConstructionValidator const &getConstructionValidator()
    {
        using SR = SettingRule;
        // clang-format off
        static ConstructionValidator cv({
            [](){
              return SR::forNamedParameter<Cat::Name>(SR::Usage::MANDATORY_NO_DEFAULT, "The unique name of this tiger.");
            }(),
            [](){
              return SR::forNamedParameter<Cat::Age>(SR::Usage::MANDATORY_NO_DEFAULT, "The current age of this tiger.");
            }()
            },
            {}, // empty warelist rules map
            ProjectWare::getConstructionValidator());
        // clang-format on
        return cv;
    }

    explicit Cat(ConstructionData const &cd) :
        name_(getConstructionValidator().validateNamedParameter<Name>(cd)),
        age_(getConstructionValidator().validateNamedParameter<Age>(cd))
    {}

    Cat::Type getType() const
    {
        return getTypeImpl();
    }

    Name getName() const
    {
        return name_;
    }

    Age getAge() const
    {
        return age_;
    }

private:
    virtual Cat::Type getTypeImpl() const = 0;

    const Name name_;
    Age age_;
};

} // namespace
D_DECLARE_FACTORYINTERFACE(::Cat)
D_DEFINE_FACTORYINTERFACE(::Cat)

#define D_DECLARE_CAT(REGISTERED_CLASS) static const DUTIL::ConcreteFactory<REGISTERED_CLASS, ::Cat, DUTIL::ProjectWare> factory;

#define D_DEFINE_CAT(REGISTERED_CLASS) \
    const DUTIL::ConcreteFactory<REGISTERED_CLASS, ::Cat, DUTIL::ProjectWare> REGISTERED_CLASS::factory;

namespace {

class Tiger : public Cat, public D_NAMED_CLASS(::Tiger)
{
public:
    D_DECLARE_CAT(Tiger)

    D_NAMED_ENUM(Species, AMUR, INDIAN, BENGAL)

    static ConstructionValidator const &getConstructionValidator()
    {
        using SR = SettingRule;
        // clang-format off
            static ConstructionValidator cv({
                [](){
                    SR sr = SR::forNamedEnum<Tiger::Species>(SR::Usage::MANDATORY_WITH_DEFAULT, "The species of this tiger.");
                    sr.defaultValue = Variant(Tiger::Species::AMUR);
                    return sr;
                }()
                },
                {}, // empty warelist rules map
                Cat::getConstructionValidator());
        // clang-format on
        return cv;
    }

    explicit Tiger(ConstructionData const &cd) :
        Cat(cd),
        species_(getConstructionValidator().validateNamedEnum<Tiger::Species>(cd))
    {}

private:
    virtual Cat::Type getTypeImpl() const override
    {
        return type_;
    }

    const Cat::Type type_ = Cat::Type::TIGER;
    const Species species_;
};

class Jaguar : public Cat, public D_NAMED_CLASS(::Jaguar)
{
public:
    D_DECLARE_CAT(Jaguar)

    static ConstructionValidator const &getConstructionValidator()
    {
        // clang-format off
            static ConstructionValidator cv(
                {}, // emtpy settingrule map
                {}, // empty warelistrule map
                Cat::getConstructionValidator());
        // clang-format on
        return cv;
    }

    explicit Jaguar(ConstructionData const &cd) :
        Cat(cd)
    {}

private:
    virtual Cat::Type getTypeImpl() const override
    {
        return type_;
    }

    const Cat::Type type_ = Cat::Type::TIGER;
};

D_DEFINE_CAT(Tiger)
D_DEFINE_CAT(Jaguar)

class Zoo : public DUTIL::ProjectWare, public D_NAMED_CLASS(::Zoo)
{
public:
    using CatMap = std::map<std::string, std::unique_ptr<Cat>>;
    //using ElephantMap = std::map<std::string, std::unique_ptr<Elephant>>;

    D_NAMED_ENUM(ClosingDay, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY)

    D_NAMED_STRING(Name)

    D_NAMED_LABEL(Min_Visitors)
    D_NAMED_LABEL(Max_Visitors)

    // Define the list of tigers living in the zoo
    D_NAMED_REFERENCE(CatList, Cat)
    D_NAMED_REFERENCE(SingleTiger, Tiger)

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
                SR sr = SR::forNamedEnum<ClosingDay>(SR::Usage::MANDATORY_WITH_DEFAULT, "The zoo's rest day.");
                sr.defaultValue = Variant(Zoo::ClosingDay::MONDAY);
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
            WR wr = WR::forSubobjectList<CatList>("A subobject list for cats living in the zoo.");
            wr.usage = WR::Usage::OPTIONAL;
            return wr;}(),
          [](){
            WR wr = WR::forSubobject<SingleTiger>("A subobject for a tiger living in the zoo.");
            wr.usage = WR::Usage::OPTIONAL;
            return wr;}()
        }, // end list of warelist rules
        ProjectWare::getConstructionValidator());
        // clang-format on
        return cv;
    }

    explicit Zoo(DUTIL::ConstructionData const &cd) :
        name_(getConstructionValidator().validateNamedParameter<Name>(cd)),
        off_(getConstructionValidator().validateNamedEnum<Zoo::ClosingDay>(cd)),
        min_(getConstructionValidator().validateNamedParameter<Min_Visitors>(cd)),
        max_(getConstructionValidator().validateNamedParameter<Max_Visitors>(cd))
    {
        auto catlist = getConstructionValidator().buildSubobjectList<CatList>(cd);
        for (auto &cat : catlist) {
            catmap_.emplace(cat->getName().value(), std::move(cat));
        }

        auto singleCat = getConstructionValidator().buildSubObject<SingleTiger>(cd);
        if (singleCat) {
            catmap_.emplace(singleCat->getName().value(), std::move(singleCat));
        }
    }

    bool findCat(std::string name) const
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

    ClosingDay getOffDay() const
    {
        return off_;
    }

private:
    CatMap catmap_;
    //ElephantMap elephantMap_;
    Name name_;
    ClosingDay off_;
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
        ConstructionData cd = ConstructionData().setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::TUESDAY);
        cd.s.setParameter<Zoo::Name>("Hellabrunn");
        auto cv = Zoo::getConstructionValidator();
        auto validationError = cv.check(cd);
        ASSERT_TRUE(validationError.empty());
        Zoo z = Zoo(cd);
        ASSERT_TRUE(z.getOffDay() == Zoo::ClosingDay::TUESDAY);
    }
    {
        // zoo name not in list of possible values
        ConstructionData cd = ConstructionData().setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::TUESDAY);
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
        ASSERT_TRUE(z.getOffDay() == Zoo::ClosingDay::MONDAY);
        ASSERT_TRUE(z.getName().value() == "Wilhelma");
    }
    {
        // min and max visitor numbers within the limits
        Zoo z(ConstructionData()
                  .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::FRIDAY)
                  .setParamter<Zoo::Name>("Hellabrunn")
                  .setParamter<Zoo::Min_Visitors>(50)
                  .setParamter<Zoo::Max_Visitors>(1000));
        ASSERT_TRUE(z.getName().value() == "Hellabrunn");
        ASSERT_TRUE(z.getOffDay() == Zoo::ClosingDay::FRIDAY);
        ASSERT_TRUE(z.getMinVisitors() == 50);
        ASSERT_TRUE(z.getMaxVisitors() == 1000);
    }
    {
        // min and max visitor numbers within the limits
        Zoo z(ConstructionData().setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::SUNDAY).setParamter<Zoo::Name>("Hellabrunn"));
        ASSERT_TRUE(z.getName().value() == "Hellabrunn");
        ASSERT_TRUE(z.getOffDay() == Zoo::ClosingDay::SUNDAY);
        ASSERT_TRUE(z.getMinVisitors() == 20);   // check default value
        ASSERT_TRUE(z.getMaxVisitors() == 8000); // check default value
    }
    {
        // min and max visitor numbers within the limits
        D_EXPECT_THROW(Zoo z(ConstructionData()
                                 .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::TUESDAY)
                                 .setParamter<Zoo::Name>("Hellabrunn")
                                 .setParamter<Zoo::Min_Visitors>(5)),
                       "is smaller than the allowed min value");
        D_EXPECT_THROW(Zoo z(ConstructionData()
                                 .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::TUESDAY)
                                 .setParamter<Zoo::Name>("Hellabrunn")
                                 .setParamter<Zoo::Min_Visitors>(50000)),
                       "is bigger than the allowed max value");
        D_EXPECT_THROW(Zoo z(ConstructionData()
                                 .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::TUESDAY)
                                 .setParamter<Zoo::Name>("Hellabrunn")
                                 .setParamter<Zoo::Max_Visitors>(50000)),
                       "is bigger than the allowed max value");
        D_EXPECT_THROW(Zoo z(ConstructionData()
                                 .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::TUESDAY)
                                 .setParamter<Zoo::Name>("Hellabrunn")
                                 .setParamter<Zoo::Max_Visitors>(0)),
                       "is smaller than the allowed min value");
    }
}

TEST_F(ConstructionValidatorTests, testBuildSubobject)
{
    // clang-format off
    Zoo z(ConstructionData()
              .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::TUESDAY)
              .setParamter<Zoo::Name>("Hellabrunn")
              .setParamter<Zoo::Min_Visitors>(50)
              .setParamter<Zoo::Max_Visitors>(1000)
              .addSubObject<Zoo::SingleTiger>(
                    ConstructionData()
                        .setConcreteClassParameter<Tiger>()
                        .setParamter<Cat::Name>("SingleTiger")
                        .setParamter<Cat::Age>(7))
          );
    // clang-format on
    ASSERT_TRUE(z.getOffDay() == Zoo::ClosingDay::TUESDAY);
    auto result = z.findCat("SingleTiger");
    ASSERT_TRUE(result);
}

TEST_F(ConstructionValidatorTests, testBuildSubobjectList)
{
    // clang-format off
    Zoo z(ConstructionData()
              .setEnum<Zoo::ClosingDay>(Zoo::ClosingDay::SATURDAY)
              .setParamter<Zoo::Name>("Hellabrunn")
              .setParamter<Zoo::Min_Visitors>(50)
              .setParamter<Zoo::Max_Visitors>(1000)
              .addSubObject<Zoo::CatList>(
                    ConstructionData()
                        .setConcreteClassParameter<Tiger>()
                        .setParamter<Tiger::Name>("TigerA")
                        .setParamter<Tiger::Age>(7))
              .addSubObject<Zoo::CatList>(
                    ConstructionData()
                        .setConcreteClassParameter<Tiger>()
                        .setParamter<Tiger::Name>("TigerB")
                        .setParamter<Tiger::Age>(8))
              .addSubObject<Zoo::CatList>(
                    ConstructionData()
                        .setConcreteClassParameter<Jaguar>()
                        .setParamter<Tiger::Name>("JaguarA")
                        .setParamter<Tiger::Age>(4))
              .addSubObject<Zoo::CatList>(
                    ConstructionData()
                        .setConcreteClassParameter<Jaguar>()
                        .setParamter<Tiger::Name>("JaguarB")
                        .setParamter<Tiger::Age>(4))
          );
    // clang-format on
    ASSERT_TRUE(z.getOffDay() == Zoo::ClosingDay::SATURDAY);
    ASSERT_TRUE((z.findCat("TigerA")));
    ASSERT_TRUE((z.findCat("TigerB")));
    ASSERT_TRUE((z.findCat("JaguarA")));
    ASSERT_TRUE((z.findCat("JaguarB")));
}
