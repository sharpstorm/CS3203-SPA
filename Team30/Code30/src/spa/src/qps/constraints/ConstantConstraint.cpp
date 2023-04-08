#include "ConstantConstraint.h"

ConstantConstraint::ConstantConstraint(WithArgumentPtr arg1,
                                       WithArgumentPtr arg2) :
    leftArg(std::move(arg1)), rightArg(std::move(arg2)) {}

bool ConstantConstraint::applyConstraint(SynonymProxyBuilder *variableTable,
                                         OverrideTable *overrideTable) {
  return leftArg->isStaticValueEqual(*rightArg);
}

PQLSynonymNameList ConstantConstraint::getAffectedSyns() const {
  return PQLSynonymNameList{};
}

bool ConstantConstraint::validateConstraint() const {
  return leftArg->doesReturnInteger() == rightArg->doesReturnInteger();
}
