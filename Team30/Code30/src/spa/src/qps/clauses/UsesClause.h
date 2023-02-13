#pragma once

#include <memory>
#include <string>

#include "SuchThatClause.h"
#include "ClauseArgument.h"

using std::shared_ptr;

class UsesClause: public SuchThatClause {
 private:
  ClauseArgument left;
  ClauseArgument right;

  QueryResult<int, string> evaluateLeftStatement(
      shared_ptr<PkbQueryHandler> pkbQueryHandler);
  QueryResult<string,string> evaluateLeftEntity(
      shared_ptr<PkbQueryHandler> pkbQueryHandler);
 public:
  UsesClause(ClauseArgument left, ClauseArgument right);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
  bool usesSynonym(string varName);
};
