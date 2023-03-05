#pragma once

#include <string>

#include "qps/clauses/SuchThatClause.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/AbstractTwoArgClause.h"

class ModifiesClause: public AbstractTwoArgClause {
 private:
  QueryResult<int, string> evaluateLeftStatement(
      PkbQueryHandler* pkbQueryHandler);
  QueryResult<string, string> evaluateLeftEntity(
      PkbQueryHandler* pkbQueryHandler);

 public:
  ModifiesClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
};
