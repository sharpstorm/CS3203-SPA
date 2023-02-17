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

  template <typename T>
  PQLQueryResult* generateQueryResult(QueryResult<T, string> queryResult);
  QueryResult<int, string> evaluateLeftStatement(
      shared_ptr<PkbQueryHandler> pkbQueryHandler);
  QueryResult<string, string> evaluateLeftEntity(
      shared_ptr<PkbQueryHandler> pkbQueryHandler);
 public:
  UsesClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLQueryResult* evaluateOn(shared_ptr<PkbQueryHandler> pkbQueryHandler);
  bool validateArgTypes(VariableTable *variables);
  bool usesSynonym(string varName);
};
