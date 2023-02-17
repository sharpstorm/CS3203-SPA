#pragma once

#include <memory>
#include <string>

#include "SuchThatClause.h"
#include "arguments/ClauseArgument.h"
#include "qps/common/adapters/EntityResultBuilder.h"

using std::shared_ptr;

class UsesClause: public SuchThatClause {
 private:
  ClauseArgumentPtr left;
  ClauseArgumentPtr right;

  template <typename T>
  PQLQueryResult* generateQueryResult(QueryResult<T, string> queryResult) {
    PQLQueryResult* pqlQueryResult = new PQLQueryResult();

    if (!left->isNamed() && !right->isNamed()) {
      pqlQueryResult->setIsStaticFalse(queryResult.isEmpty);
      return pqlQueryResult;
    }

    left->invokeWithName([&queryResult, &pqlQueryResult](PQLSynonymName name){
      EntityResult result = EntityResultBuilder::buildEntityResult(true, queryResult);
      pqlQueryResult->addToEntityMap(name, result);
    });

    right->invokeWithName([&queryResult, &pqlQueryResult](PQLSynonymName name){
      EntityResult result = EntityResultBuilder::buildEntityResult(false, queryResult);
      pqlQueryResult->addToEntityMap(name, result);
    });

    return pqlQueryResult;
  };

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
