#pragma once

#include <memory>
#include <string>

#include "SuchThatClause.h"
#include "arguments/ClauseArgument.h"
#include "AbstractTwoArgClause.h"

using std::shared_ptr;

class ModifiesClause: public AbstractTwoArgClause {
 private:
  QueryResult<int, string> evaluateLeftStatement(
      shared_ptr<PkbQueryHandler> pkbQueryHandler);
  QueryResult<string, string> evaluateLeftEntity(
      shared_ptr<PkbQueryHandler> pkbQueryHandler);

 public:
  ModifiesClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
};
