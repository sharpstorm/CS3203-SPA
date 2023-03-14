#pragma once

#include "Constraint.h"

template <typename Constant>

class ConstantConstraint : virtual public Constraint {
 private:
  Constant leftArg;
  Constant rightArg;
 public:
  ConstantConstraint(Constant arg1, Constant arg2) : leftArg(arg1), rightArg(arg2) {}
  bool applyConstraint(VariableTable* variableTable, OverrideTable* overrideTable) {
    return leftArg == rightArg;
  };
};