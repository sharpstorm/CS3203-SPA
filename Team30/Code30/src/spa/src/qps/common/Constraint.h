#pragma once

#include <memory>

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
  bool validateConstraints();
};

typedef shared_ptr<Constraint> ConstraintSPtr;
