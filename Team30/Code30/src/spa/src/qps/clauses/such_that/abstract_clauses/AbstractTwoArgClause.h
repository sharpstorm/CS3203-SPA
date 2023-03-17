#pragma once

#include <unordered_set>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "common/Types.h"
#include "qps/clauses/Clause.h"
#include "qps/clauses/InvokerTypes.h"
#include "qps/clauses/SuchThatClause.h"

using std::unordered_set;

class AbstractTwoArgClause: public SuchThatClause {
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
                   RightResultType, RightArgType> diffSynInvoker,
      SymmetricQueryInvoker<LeftResultType, LeftArgType> sameSynInvoker>
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) {
    if (isSameSynonym()) {
      auto queryResult = sameSynInvoker(pkbQueryHandler,
                                        leftTransformer(left.get()));
      return Clause::toQueryResult(left->getName(), queryResult);
    }

    auto queryResult = diffSynInvoker(
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