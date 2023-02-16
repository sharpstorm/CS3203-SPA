#pragma once

#include <memory>
#include <string>

#include "SuchThatClause.h"
#include "arguments/ClauseArgument.h"

using std::shared_ptr;

class UsesClause: public SuchThatClause {
 private:
  ClauseArgumentPtr left;
  ClauseArgumentPtr right;

 public:
  UsesClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
  bool usesSynonym(string varName);
};
