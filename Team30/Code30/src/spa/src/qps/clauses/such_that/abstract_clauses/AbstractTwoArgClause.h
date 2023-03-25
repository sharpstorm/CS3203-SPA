#pragma once

#include <unordered_set>

#include "qps/clauses/arguments/ClauseArgument.h"
#include "common/Types.h"
#include "qps/clauses/Clause.h"
#include "qps/clauses/InvokerTypes.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/clauses/ClauseScoring.h"

using std::unordered_set;

class AbstractTwoArgClause: public SuchThatClause {
 protected:
  ClauseArgumentPtr left;
  ClauseArgumentPtr right;

  bool isSameSynonym();

  template <
      typename LeftResultType, typename LeftArgType,
      typename RightResultType, typename RightArgType>
  PQLQueryResult* abstractEvaluateOn(
      const QueryExecutorAgent &agent,
      ArgumentTransformer<LeftArgType> leftTransformer,
      ArgumentTransformer<RightArgType> rightTransformer,
      QueryInvoker<LeftResultType, LeftArgType,
                   RightResultType, RightArgType> diffSynInvoker,
      SymmetricQueryInvoker<LeftResultType, LeftArgType> sameSynInvoker) {
    if (isSameSynonym()) {
      auto ref = leftTransformer(left.get());
      ref = agent.transformArg(left->getName(), ref);
      if (!agent.isValid(ref)) {
        return new PQLQueryResult();
      }

      auto queryResult = sameSynInvoker(agent, ref);
      return Clause::toQueryResult(left->getName(), queryResult);
    }

    LeftArgType leftArg = leftTransformer(left.get());
    RightArgType rightArg = rightTransformer(right.get());

    leftArg = agent.transformArg(left->getName(), leftArg);
    rightArg = agent.transformArg(right->getName(), rightArg);
    if (!agent.isValid(leftArg) || !agent.isValid(rightArg)) {
      return new PQLQueryResult();
    }

    auto queryResult = diffSynInvoker(agent, leftArg, rightArg);
    return Clause::toQueryResult(left.get(), right.get(), queryResult);
  }

  template <SynonymPredicate leftValidator, SynonymPredicate rightValidator>
  bool validateArgTypes(VariableTable *variables) {
    bool isLeftValid = left->synonymSatisfies(leftValidator);
    bool isRightValid = right->synonymSatisfies(rightValidator);
    return isLeftValid && isRightValid;
  }

  template <
      ComplexityScore constantModifier,
      ComplexityScore oneSynModifier,
      ComplexityScore twoSynModifier>
  ComplexityScore computeComplexityScore(const OverrideTable *table) {
    bool isLeftConstant = left->isConstant()
        || table->contains(left->getName());
    bool isRightConstant = right->isConstant()
        || table->contains(right->getName());

    if (isLeftConstant && isRightConstant) {
      return COMPLEXITY_QUERY_CONSTANT + constantModifier;
    } else if (!isLeftConstant && !isRightConstant) {
      return COMPLEXITY_QUERY_LIST_ALL + twoSynModifier +
          + left->getSynComplexity() + right->getSynComplexity();
    } else if (isLeftConstant) {
      return right->getSynComplexity() + oneSynModifier;
    } else {
      return left->getSynComplexity() + oneSynModifier;
    }
  }

  template <
      ComplexityScore constantModifier,
      ComplexityScore oneSynModifier,
      ComplexityScore twoSynModifier>
  ComplexityScore computeNoSymmetryComplexityScore(const OverrideTable *table) {
    if (isSameSynonym()) {
      return COMPLEXITY_QUERY_CONSTANT;
    }

    return computeComplexityScore<constantModifier,
                                  oneSynModifier,
                                  twoSynModifier>(table);
  }

  ComplexityScore computeNoSymmetryComplexityScore(const OverrideTable *table) {
    if (isSameSynonym()) {
      return COMPLEXITY_QUERY_CONSTANT;
    }

    return computeComplexityScore(table);
  }

  ComplexityScore computeComplexityScore(const OverrideTable *table);

 public:
  AbstractTwoArgClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  SynonymList getUsedSynonyms() override;
};
