#pragma once

#include <utility>

#include "qps/clauses/abstract_clauses/AbstractStmtStmtClause.h"
#include "qps/clauses/SuchThatClause.h"

typedef StmtStmtInvoker NextInvoker;

template <NextInvoker invoker>
using AbstractNextClause = AbstractStmtStmtClause<
    invoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

constexpr NextInvoker nextInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                       const StmtRef &leftArg,
                                       const StmtRef &rightArg){
  return QueryResult<StmtValue, StmtValue>{};
};

constexpr NextInvoker nextTInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                        const StmtRef &leftArg,
                                        const StmtRef &rightArg){
  return QueryResult<StmtValue, StmtValue>{};
};

class NextClause: public AbstractNextClause<nextInvoker>,
                  public SuchThatClause  {
 public:
  NextClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};

class NextTClause: public AbstractNextClause<nextTInvoker>,
                   public SuchThatClause  {
 public:
  NextTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};
