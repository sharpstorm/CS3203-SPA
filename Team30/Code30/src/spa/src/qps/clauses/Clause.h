#pragma once

#include <string>

#include "qps/common/IEvaluatable.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/common/adapters/StatementResultBuilder.h"
#include "qps/common/adapters/EntityResultBuilder.h"

class Clause : public IEvaluatable {
 public:
  virtual ~Clause() = default;
  virtual bool validateArgTypes(VariableTable* variables) = 0;

  static PQLQueryResult *stmtQueryToQueryResult(ClauseArgument* left,
                                         ClauseArgument* right,
                                         QueryResult<int, int> queryResult) {
    PQLQueryResult* pqlQueryResult = new PQLQueryResult();
    if (!left->isNamed() && !right->isNamed()) {
      pqlQueryResult->setIsStaticFalse(queryResult.isEmpty);
      return pqlQueryResult;
    }

    left->invokeWithName([&queryResult, &pqlQueryResult](PQLSynonymName name){
      StatementResult result =
          StatementResultBuilder::buildStatementResult(true, queryResult);
      pqlQueryResult->addToStatementMap(name, result);
    });

    right->invokeWithName([&queryResult, &pqlQueryResult](PQLSynonymName name){
      StatementResult result =
          StatementResultBuilder::buildStatementResult(false, queryResult);
      pqlQueryResult->addToStatementMap(name, result);
    });

    return pqlQueryResult;
  }

  template<typename T>
  static PQLQueryResult *entityQueryToQueryResult(ClauseArgument* left,
                                           ClauseArgument* right,
                                           QueryResult<T, string> queryResult) {
    PQLQueryResult* pqlQueryResult = new PQLQueryResult();
    if (!left->isNamed() && !right->isNamed()) {
      pqlQueryResult->setIsStaticFalse(queryResult.isEmpty);
      return pqlQueryResult;
    }

    left->invokeWithName(
        [&queryResult, &pqlQueryResult](PQLSynonymName name){
      EntityResult result =
          EntityResultBuilder::buildEntityResult(true, queryResult);
      pqlQueryResult->addToEntityMap(name, result);
    });

    right->invokeWithName(
        [&queryResult, &pqlQueryResult](PQLSynonymName name){
      EntityResult result =
          EntityResultBuilder::buildEntityResult(false, queryResult);
      pqlQueryResult->addToEntityMap(name, result);
    });

    return pqlQueryResult;
  }
};
