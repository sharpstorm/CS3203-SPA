#pragma once

#include <functional>
#include <memory>
#include "../../common/PQLQuerySynonym.h"
#include "common/Types.h"

using std::unique_ptr;

typedef bool (*SynonymPredicate)(PQLQuerySynonym syn);

class ClauseArgument {
 public:
  virtual ~ClauseArgument() = default;
  virtual bool synonymSatisfies(SynonymPredicate predicate);

  virtual bool isNamed();
  virtual bool isWildcard();

  virtual PQLSynonymName getName();
  virtual StmtRef toStmtRef() = 0;
  virtual EntityRef toEntityRef() = 0;

  static bool isStatement(PQLQuerySynonym syn);
  template<PQLSynonymType TYPE>
  static bool isType(PQLQuerySynonym syn) {
    return syn.isType(TYPE);
  };
  template<typename condA, typename condB>
  static bool isEither(PQLQuerySynonym syn) {
    return condA(syn) || condB(syn);
  }
};

typedef unique_ptr<ClauseArgument> ClauseArgumentPtr;
