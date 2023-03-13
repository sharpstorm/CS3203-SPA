#pragma once

#include <utility>

#include "qps/clauses/arguments/WithArgument.h"

using std::shared_ptr;

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

typedef shared_ptr<Constraint> ConstraintSPtr;
