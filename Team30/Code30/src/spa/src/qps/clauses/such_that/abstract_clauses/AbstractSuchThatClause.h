#pragma once

#include <utility>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "pkb/queryHandlers/PkbQueryHandler.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/clauses/AbstractTwoArgClause.h"
#include "qps/clauses/SuchThatClause.h"

template <typename LT, typename LA, typename RT, typename RA>
using QueryInvoker = QueryResult<LT, RT>(*)(
    PkbQueryHandler* pkbQueryHandler,
    LA leftArg,
    RA rightArg);

template <typename ResultType>
using ArgumentTransformer = ResultType(*)(ClauseArgument* arg);

template <
    typename LT, typename LA,
    typename RT, typename RA>
class AbstractSuchThatClause:
    public AbstractTwoArgClause {
 public:
  AbstractSuchThatClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractTwoArgClause(std::move(left), std::move(right)) {
  }

 protected:
  template <
      ArgumentTransformer<LA> leftTransformer,
      ArgumentTransformer<RA> rightTransformer,
      QueryInvoker<LT, LA, RT, RA> invoker>
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) {
    QueryResult<LT, RT> queryResult = invoker(
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

