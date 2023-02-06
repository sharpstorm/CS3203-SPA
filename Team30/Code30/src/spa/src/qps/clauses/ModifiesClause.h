#pragma once

#include <memory>
#include <string>

#include "SuchThatClause.h"
#include "ClauseArgument.h"

using std::shared_ptr;

class ModifiesClause: public SuchThatClause {
 private:
  ClauseArgument left;
  ClauseArgument right;

 public:
  ModifiesClause(ClauseArgument left, ClauseArgument right);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
  bool usesSynonym(string varName);
};
