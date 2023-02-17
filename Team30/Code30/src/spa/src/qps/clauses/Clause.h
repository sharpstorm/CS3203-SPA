#pragma once

#include <unordered_set>

#include "qps/common/IEvaluatable.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/common/adapters/StatementResultBuilder.h"
#include "qps/common/adapters/EntityResultBuilder.h"

using std::unordered_set;

class Clause : public IEvaluatable {
 public:
  virtual ~Clause() = default;
  virtual bool validateArgTypes(VariableTable* variables) = 0;

 protected:
  template<class T, class U>
  static PQLQueryResult *toQueryResult(ClauseArgument* left,
                                         ClauseArgument* right,
                                         QueryResult<T, U> queryResult) {
    PQLQueryResult* pqlQueryResult = new PQLQueryResult();
    if (!left->isNamed() && !right->isNamed()) {
      pqlQueryResult->setIsStaticFalse(queryResult.isEmpty);
      return pqlQueryResult;
    } else if (left->isNamed() && right->isNamed()) {
      pqlQueryResult->add(left->getName(), right->getName(),
                          queryResult.pairVals);
    } else if (left->isNamed()) {
      pqlQueryResult->add(left->getName(), queryResult.firstArgVals);
    } else if (right->isNamed()) {
      pqlQueryResult->add(right->getName(), queryResult.secondArgVals);
    }

    return pqlQueryResult;
  }

  template<class T>
  static PQLQueryResult *toQueryResult(const PQLSynonymName &name,
                                       unordered_set<T> queryResult) {
    PQLQueryResult* pqlQueryResult = new PQLQueryResult();
    pqlQueryResult->add(name, queryResult);
    return pqlQueryResult;
  }
};
