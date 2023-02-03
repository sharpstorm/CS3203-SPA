#pragma once

#include "SuchThatClause.h"
#include "ClauseArgument.h"

class ParentTClause: public SuchThatClause {
 private:
  ClauseArgument left;
  ClauseArgument right;

 public:
  ParentTClause(ClauseArgument left, ClauseArgument right);
  QueryResult* evaluateOn();
  bool validateArgTypes(VariableTable *variables);
};
