#pragma once

#include <memory>
#include <string>

#include "qps/clauses/SuchThatClause.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/AbstractTwoArgClause.h"

using std::shared_ptr;

class UsesClause: public AbstractTwoArgClause {
 private:
  QueryResult<int, string> evaluateLeftStatement(
      shared_ptr<PkbQueryHandler> pkbQueryHandler);
  QueryResult<string, string> evaluateLeftEntity(
      shared_ptr<PkbQueryHandler> pkbQueryHandler);

 public:
  UsesClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
};
