#pragma once

#include <unordered_set>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "common/Types.h"
#include "qps/clauses/Clause.h"
#include "qps/clauses/SuchThatClause.h"
#include "InvokerTypes.h"

using std::unordered_set;

class AbstractTwoArgClause: virtual public Clause {
 protected:
  ClauseArgumentPtr left;
  ClauseArgumentPtr right;

  bool isSameSynonym();

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

 public:
  AbstractTwoArgClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  SynonymList getUsedSynonyms() override;
};
