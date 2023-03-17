#pragma once

#include <utility>
#include <unordered_set>

#include "abstract_clauses/AbstractStmtStmtClause.h"
#include "qps/clauses/SuchThatClause.h"

typedef StmtStmtInvoker AffectsInvoker;
typedef StmtInvoker AffectsSameSynInvoker;

template <AffectsInvoker invoker, AffectsSameSynInvoker sameSynInvoker>
using AbstractAffectsClause = AbstractStmtStmtClause<
    invoker,
    sameSynInvoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

constexpr AffectsInvoker affectsInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                             OverrideTable * table,
                                             const StmtRef &leftArg,
                                             const StmtRef &rightArg){
  return QueryResult<StmtValue, StmtValue>{};
};

constexpr AffectsInvoker affectsTInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                              OverrideTable * table,
                                              const StmtRef &leftArg,
                                              const StmtRef &rightArg){
  return QueryResult<StmtValue, StmtValue>{};
};

constexpr AffectsSameSynInvoker affectsSymmetricInvoker =
    [](PkbQueryHandler* pkbQueryHandler, OverrideTable* table,
       const StmtRef &arg){
      return unordered_set<StmtValue>{};
    };

class AffectsClause: public AbstractAffectsClause<
    affectsInvoker,
    affectsSymmetricInvoker> {
 public:
  AffectsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};

class AffectsTClause: public AbstractAffectsClause<
    affectsTInvoker,
    affectsSymmetricInvoker> {
 public:
  AffectsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};
