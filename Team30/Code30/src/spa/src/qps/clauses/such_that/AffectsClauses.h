#pragma once

#include <utility>

#include "qps/clauses/abstract_clauses/AbstractStmtStmtClause.h"
#include "qps/clauses/SuchThatClause.h"

typedef StmtStmtInvoker AffectsInvoker;

template <AffectsInvoker invoker>
using AbstractAffectsClause = AbstractStmtStmtClause<
    invoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

constexpr AffectsInvoker affectsInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                             const StmtRef &leftArg,
                                             const StmtRef &rightArg){
  return QueryResult<StmtValue, StmtValue>{};
};

constexpr AffectsInvoker affectsTInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                              const StmtRef &leftArg,
                                              const StmtRef &rightArg){
  return QueryResult<StmtValue, StmtValue>{};
};

class AffectsClause: public AbstractAffectsClause<affectsInvoker>,
                     public SuchThatClause  {
 public:
  AffectsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};

class AffectsTClause: public AbstractAffectsClause<affectsTInvoker>,
                      public SuchThatClause  {
 public:
  AffectsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};
