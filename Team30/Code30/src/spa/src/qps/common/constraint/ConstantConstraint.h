#pragma once

#include <utility>

#include "Constraint.h"
#include "qps/clauses/arguments/WithArgument.h"

class ConstantConstraint : virtual public Constraint {
 private:
  WithArgumentPtr leftArg;
  WithArgumentPtr rightArg;
 public:
  ConstantConstraint(WithArgumentPtr arg1, WithArgumentPtr arg2)
      : leftArg(std::move(arg1)), rightArg(std::move(arg2)) {}
  bool applyConstraint(VariableTable* variableTable,
                       OverrideTable* overrideTable,
                       PkbQueryHandler* queryHandler) override {
    return leftArg->isStaticValueEqual(*rightArg);
  }

  bool validateConstraint() {
    return leftArg->doesReturnInteger() == rightArg->doesReturnInteger();
  }
};
