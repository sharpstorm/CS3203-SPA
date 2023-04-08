#pragma once

#include "qps/clauses/arguments/ClauseArgument.h"
#include "common/Types.h"
#include "qps/clauses/Clause.h"
#include "qps/clauses/InvokerTypes.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/clauses/ClauseScoring.h"
#include "qps/common/intermediate_result/PQLQueryResultBuilder.h"

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

      QueryResultSet<LeftResultType> queryResult = sameSynInvoker(agent, ref);
      PQLQueryResultBuilder<LeftResultType, RightResultType> builder;
      builder.setLeftName(left.get());
      builder.setLeftRef(ref);
      return builder.build(queryResult);
    }

    LeftArgType leftArg = leftTransformer(left.get());
    RightArgType rightArg = rightTransformer(right.get());

    leftArg = agent.transformArg(left->getName(), leftArg);
    rightArg = agent.transformArg(right->getName(), rightArg);
    if (!agent.isValid(leftArg) || !agent.isValid(rightArg)) {
      return new PQLQueryResult();
    }

    auto queryResult = diffSynInvoker(agent, leftArg, rightArg);
    PQLQueryResultBuilder<LeftResultType, RightResultType> builder;
    builder.setLeftName(left.get());
    builder.setRightName(right.get());
    builder.setLeftRef(leftArg);
    builder.setRightRef(rightArg);
    return builder.build(queryResult.get());
  }

  template<SynonymPredicate leftValidator, SynonymPredicate rightValidator>
  bool validateArgTypes() const {
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
          left->getSynComplexity() + right->getSynComplexity();
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
  PQLSynonymNameList getUsedSynonyms() const override;
};
