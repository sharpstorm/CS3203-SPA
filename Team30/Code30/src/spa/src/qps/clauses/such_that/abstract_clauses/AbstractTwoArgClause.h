#pragma once

#include "qps/clauses/arguments/ClauseArgument.h"
#include "common/Types.h"
#include "qps/clauses/Clause.h"
#include "qps/clauses/InvokerTypes.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/clauses/ClauseScoring.h"

class AbstractTwoArgClause : public SuchThatClause {
 protected:
  ClauseArgumentPtr left;
  ClauseArgumentPtr right;

  bool isSameSynonym() const;

  template<
      typename LeftResultType, typename LeftArgType,
      typename RightResultType, typename RightArgType>
  PQLQueryResult *abstractEvaluateOn(
      const QueryExecutorAgent &agent,
      ArgumentTransformer<LeftArgType> leftTransformer,
      ArgumentTransformer<RightArgType> rightTransformer,
      QueryInvoker<LeftResultType, LeftArgType,
                   RightResultType, RightArgType> diffSynInvoker,
      SymmetricQueryInvoker<LeftResultType, LeftArgType> sameSynInvoker) const {
    if (isSameSynonym()) {
      auto baseRef = leftTransformer(left.get());
      auto ref = agent.transformArg(left->getName(), baseRef);
      if (!agent.isValid(ref)) {
        return new PQLQueryResult();
      }

      auto queryResult = sameSynInvoker(agent, ref);
      return Clause::toQueryResult(left->getName(), queryResult);
    }

    LeftArgType leftArg = leftTransformer(left.get());
    RightArgType rightArg = rightTransformer(right.get());

    LeftArgType leftTransformed = agent.transformArg(left->getName(),
                                                     leftArg);
    RightArgType rightTransformed = agent.transformArg(right->getName(),
                                                       rightArg);
    if (!agent.isValid(leftTransformed) || !agent.isValid(rightTransformed)) {
      return new PQLQueryResult();
    }

    auto queryResult = diffSynInvoker(agent, leftTransformed, rightTransformed);
    return Clause::toQueryResult(left.get(), right.get(), queryResult.get());
  }

  template<SynonymPredicate leftValidator, SynonymPredicate rightValidator>
  bool validateArgTypes(const VariableTable *variables) const {
    bool isLeftValid = left->synonymSatisfies(leftValidator);
    bool isRightValid = right->synonymSatisfies(rightValidator);
    return isLeftValid && isRightValid;
  }

  template<
      ComplexityScore constantModifier,
      ComplexityScore oneSynModifier,
      ComplexityScore twoSynModifier>
  ComplexityScore computeComplexityScore(const OverrideTable *table) const {
    bool isLeftConstant = left->isConstant()
        || table->contains(left->getName());
    bool isRightConstant = right->isConstant()
        || table->contains(right->getName());

    if (isLeftConstant && isRightConstant) {
      return COMPLEXITY_QUERY_CONSTANT + constantModifier;
    } else if (!isLeftConstant && !isRightConstant) {
      return COMPLEXITY_QUERY_LIST_ALL + twoSynModifier +
          +left->getSynComplexity() + right->getSynComplexity();
    } else if (isLeftConstant) {
      return right->getSynComplexity() + oneSynModifier;
    } else {
      return left->getSynComplexity() + oneSynModifier;
    }
  }

  template<
      ComplexityScore constantModifier,
      ComplexityScore oneSynModifier,
      ComplexityScore twoSynModifier>
  ComplexityScore computeNoSymmetryComplexityScore(const OverrideTable *table)
  const {
    if (isSameSynonym()) {
      return COMPLEXITY_QUERY_CONSTANT;
    }

    return computeComplexityScore<constantModifier,
                                  oneSynModifier,
                                  twoSynModifier>(table);
  }

  ComplexityScore computeNoSymmetryComplexityScore(const OverrideTable *table)
  const {
    if (isSameSynonym()) {
      return COMPLEXITY_QUERY_CONSTANT;
    }

    return computeComplexityScore(table);
  }

  ComplexityScore computeComplexityScore(const OverrideTable *table) const;

 public:
  AbstractTwoArgClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  const PQLSynonymNameList getUsedSynonyms() const override;
};
