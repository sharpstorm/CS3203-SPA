#pragma once

#include <memory>

#include "common/Types.h"
#include "../../common/PQLQuerySynonym.h"
#include "qps/common/constraint/Constraint.h"

using std::unique_ptr;

typedef bool (*SynonymPredicate)(PQLQuerySynonym syn);

class ClauseArgument {
 public:
  virtual ~ClauseArgument() = default;
  virtual bool synonymSatisfies(SynonymPredicate predicate);

  virtual bool isNamed();
  virtual bool isWildcard();

  virtual PQLSynonymName getName();
  virtual PQLQuerySynonym* getSyn();
  virtual StmtRef toStmtRef() = 0;
  virtual EntityRef toEntityRef() = 0;

  static bool isStatement(PQLQuerySynonym syn);
  bool canSubstitute(OverrideTable* table);

  static StmtType getStmtTypeFromSynType(PQLSynonymType type);
  static EntityType getEntTypeFromSynType(PQLSynonymType type);
  bool existsInPKB(PkbQueryHandler* handler,
                          OverrideTransformer trans);

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
