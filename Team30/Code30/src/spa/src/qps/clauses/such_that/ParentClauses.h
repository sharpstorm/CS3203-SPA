#pragma once

#include <utility>

#include "qps/clauses/abstract_clauses/AbstractStmtStmtClause.h"

typedef StmtStmtInvoker ParentInvoker;

template <ParentInvoker invoker>
using AbstractParentClause = AbstractStmtStmtClause<
    invoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

constexpr ParentInvoker parentInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                           const StmtRef &leftArg,
                                           const StmtRef &rightArg){
  return pkbQueryHandler->queryParent(leftArg, rightArg);
};

constexpr ParentInvoker parentTInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                            const StmtRef &leftArg,
                                            const StmtRef &rightArg){
  return pkbQueryHandler->queryParentStar(leftArg, rightArg);
};

class ParentClause: public AbstractParentClause<parentInvoker>,
                    public SuchThatClause  {
 public:
  ParentClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};

class ParentTClause: public AbstractParentClause<parentTInvoker>,
                     public SuchThatClause  {
 public:
  ParentTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};
