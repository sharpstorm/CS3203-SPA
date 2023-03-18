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
  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler, OverrideTable* table) {
    if (isSameSynonym()) {
      auto queryResult = sameSynInvoker(pkbQueryHandler,
                                        leftTransformer(left.get()));
      return Clause::toQueryResult(left->getName(), queryResult);
    }

    LeftArgType leftArg = leftTransformer(left.get());
    RightArgType rightArg = rightTransformer(right.get());
    if (left->canSubstitute(table)) {
      OverrideTransformer overrideTrans = table->at(left->getName());
      leftArg = overrideTrans.transformArg(leftArg);
     if (!Clause::isValidRef(leftArg, pkbQueryHandler)) {
        return new PQLQueryResult();
      }
    }

    if (right->canSubstitute(table)) {
      OverrideTransformer overrideTrans = table->at(right->getName());
      rightArg = overrideTrans.transformArg(rightArg);
      if (!Clause::isValidRef(rightArg, pkbQueryHandler)) {
        return new PQLQueryResult();
      }
    }

    auto queryResult = diffSynInvoker(
        pkbQueryHandler, leftArg, rightArg);

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
