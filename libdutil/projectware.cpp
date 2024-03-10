#include "projectware.h"
#include <iostream>
#include "constructionvalidator.h"
using namespace DUTIL;

D_DEFINE_FACTORYINTERFACE(::DUTIL::ProjectWare);

//ConstructionValidator const& ProjectWare::getConstructionValidator()
//{
//  using SR = SettingRule;
//  static const ConstructionValidator cv = ConstructionValidator({[]() {
//    return SR::forNamedParameter<Ware::Type>(
//        SR::Usage::OPTIONAL, "Set the type of the object these construction data are meant for.");
//  }()});
//  return cv;
//}
