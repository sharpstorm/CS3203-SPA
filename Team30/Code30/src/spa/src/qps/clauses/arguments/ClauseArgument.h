#pragma once

#include <memory>
#include <string>

#include "common/Types.h"
#include "../../common/PQLQuerySynonym.h"
#include "qps/constraints/Constraint.h"
#include "qps/common/IEvaluatable.h"

using std::unique_ptr, std::to_string, std::string;

typedef bool (*SynonymPredicate)(PQLQuerySynonym syn);

class ClauseArgument {
 public:
  virtual ~ClauseArgument() = default;
  virtual bool synonymSatisfies(SynonymPredicate predicate);

  virtual bool isNamed();
  virtual bool isWildcard();
  virtual bool isConstant();

  virtual PQLSynonymName getName();
  virtual ComplexityScore getSynComplexity();
  virtual StmtRef toStmtRef() = 0;
  virtual EntityRef toEntityRef() = 0;

  static bool isStatement(PQLQuerySynonym syn);

  template<PQLSynonymType TYPE>
  static bool isType(PQLQuerySynonym syn) {
    return syn.isType(TYPE);
  }

  template<SynonymPredicate condA, SynonymPredicate condB>
  static bool isEither(PQLQuerySynonym syn) {
    return condA(syn) || condB(syn);
  }
};

typedef unique_ptr<ClauseArgument> ClauseArgumentPtr;
