#pragma once

#include <utility>
#include <unordered_set>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/clauses/abstract_clauses/AbstractTwoArgClause.h"
#include "qps/clauses/SuchThatClause.h"

using std::unordered_set;

template <
    typename LeftResultType, typename LeftArgType,
    typename RightResultType, typename RightArgType>
using QueryInvoker = QueryResult<LeftResultType, RightResultType>(*)(
    PkbQueryHandler* pkbQueryHandler,
    const LeftArgType &leftArg,
    const RightArgType &rightArg);

template <typename ResultType>
using ArgumentTransformer = ResultType(*)(ClauseArgument* arg);

class AbstractSuchThatClause:
    public AbstractTwoArgClause {
 public:
  AbstractSuchThatClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractTwoArgClause(std::move(left), std::move(right)) {
  }

 protected:
  template <
      typename LeftResultType, typename LeftArgType,
      typename RightResultType, typename RightArgType,
      ArgumentTransformer<LeftArgType> leftTransformer,
      ArgumentTransformer<RightArgType> rightTransformer,
      QueryInvoker<LeftResultType, LeftArgType,
                   RightResultType, RightArgType> invoker>
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) {
    if (isSameSynonym()) {
      return Clause::toQueryResult(left->getName(),
                                   unordered_set<LeftResultType>{});
    }

    auto queryResult = invoker(
        pkbQueryHandler,
        leftTransformer(left.get()),
        rightTransformer(right.get()));
    return Clause::toQueryResult(left.get(), right.get(), queryResult);
  }

  template <SynonymPredicate leftValidator, SynonymPredicate rightValidator>
  bool validateArgTypes(VariableTable *variables) {
    bool isLeftValid = left->synonymSatisfies(leftValidator);
    bool isRightValid = right->synonymSatisfies(rightValidator);
    return isLeftValid && isRightValid;
  }
};

