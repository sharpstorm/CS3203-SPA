#pragma once

#include "qps/clauses/arguments/WithArgument.h"

class Constraint {
  WithArgumentPtr leftArg;
  WithArgumentPtr rightArg;

 public:
  Constraint(WithArgumentPtr left, WithArgumentPtr right);
  bool isStaticArguments();
  bool replaceSyns(VariableTable variables);
  bool mapSynToStatic();
  bool applyConstraint(VariableTable variables);
};