#ifndef LIBD_TESTS_ZOO_H
#define LIBD_TESTS_ZOO_H
#include "cat.h"

namespace TESTS {

class Zoo : public DUTIL::ProjectWare, public D_NAMED_CLASS(::TESTS::Zoo)
{
public:
    using CatMap = std::map<std::string, std::unique_ptr<Cat>>;

    D_NAMED_ENUM(ClosingDay, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY)

    D_NAMED_STRING(Name)

    D_NAMED_LABEL(Min_Visitors)
    D_NAMED_LABEL(Max_Visitors)

    // Define the list of tigers living in the zoo
    D_NAMED_REFERENCE(CatList, Cat)
    D_NAMED_REFERENCE(SingleTiger, Tiger)

    static DUTIL::ConstructionValidator const &getConstructionValidator();

    explicit Zoo(DUTIL::ConstructionData const &cd);

    bool findCat(std::string name) const;

    Name getName() const;

    Min_Visitors getMinVisitors() const;

    Max_Visitors getMaxVisitors() const;

    ClosingDay getOffDay() const;

private:
    CatMap catmap_;
    Name name_;
    ClosingDay off_;
    Min_Visitors min_;
    Max_Visitors max_;
};

} // namespace TESTS
#endif // LIBD_TESTS_ZOO_H
