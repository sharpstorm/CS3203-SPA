#pragma once

#include <memory>

#include "common/Types.h"
#include "../../common/PQLQuerySynonym.h"
#include "qps/constraints/Constraint.h"
#include "qps/common/IEvaluatable.h"

using std::unique_ptr;

typedef bool (*SynonymPredicate)(const PQLQuerySynonym &syn);

class ClauseArgument {
 public:
  virtual ~ClauseArgument() = default;
  virtual bool synonymSatisfies(SynonymPredicate predicate) const;

  virtual bool isNamed() const;
  virtual bool isWildcard() const;
  virtual bool isConstant() const;

  virtual PQLSynonymName getName() const;
  virtual ComplexityScore getSynComplexity() const;
  virtual StmtRef toStmtRef() const = 0;
  virtual EntityRef toEntityRef() const = 0;

  static bool isStatement(const PQLQuerySynonym &syn);

  template<PQLSynonymType TYPE>
  static bool isType(const PQLQuerySynonym &syn) {
    return syn.isType(TYPE);
  }

  template<SynonymPredicate condA, SynonymPredicate condB>
  static bool isEither(const PQLQuerySynonym &syn) {
    return condA(syn) || condB(syn);
  }
};

typedef unique_ptr<ClauseArgument> ClauseArgumentPtr;
