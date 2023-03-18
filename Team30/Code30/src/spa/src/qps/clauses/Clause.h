#pragma once

#include <unordered_set>
#include <memory>

#include "qps/common/IEvaluatable.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/clauses/arguments/ClauseArgument.h"

using std::unordered_set, std::shared_ptr;

class Clause : public IEvaluatable {
 public:
  virtual bool validateArgTypes(VariableTable* variables) = 0;

 protected:
  static EntityRef toEntityRef(ClauseArgument* arg) {
    return arg->toEntityRef();
  }

  static StmtRef toStmtRef(ClauseArgument* arg) {
    return arg->toStmtRef();
  }

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

  template<class T, class U>
  static PQLQueryResult *toQueryResult(const PQLSynonymName &left,
                                       ClauseArgument* right,
                                       QueryResult<T, U> queryResult) {
    PQLQueryResult* pqlQueryResult = new PQLQueryResult();
    if (right->isNamed()) {
      pqlQueryResult->add(left, right->getName(), queryResult.pairVals);
    } else {
      pqlQueryResult->add(left, queryResult.firstArgVals);
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

typedef shared_ptr<Clause> ClauseSPtr;
