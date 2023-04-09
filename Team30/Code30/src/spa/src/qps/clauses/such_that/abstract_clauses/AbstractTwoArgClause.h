#pragma once

#include "qps/clauses/arguments/ClauseArgument.h"
#include "common/Types.h"
#include "qps/clauses/Clause.h"
#include "qps/clauses/InvokerTypes.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/clauses/ClauseScoring.h"
#include "qps/common/intermediate_result/PQLQueryResultBuilder.h"

/*
 * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur
 */

class AbstractTwoArgClause : public SuchThatClause {
 protected:
  ClauseArgumentPtr left;
  ClauseArgumentPtr right;

  bool isSameSynonym() const;

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
  const;

  ComplexityScore computeComplexityScore(const OverrideTable *table) const;

 public:
  AbstractTwoArgClause(ClauseArgumentPtr left, ClauseArgumentPtr right);
  PQLSynonymNameList getUsedSynonyms() const override;
};
