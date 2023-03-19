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
  bool applyConstraint(VariableTableProxyBuilder* variableTable,
                       OverrideTable* overrideTable) override {
    return leftArg->isStaticValueEqual(*rightArg);
  }

  bool validateConstraint() override {
    return leftArg->doesReturnInteger() == rightArg->doesReturnInteger();
  }
};
