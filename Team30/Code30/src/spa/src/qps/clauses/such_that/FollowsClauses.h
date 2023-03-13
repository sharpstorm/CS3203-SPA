#pragma once

#include <utility>

#include "abstract_clauses/AbstractStmtStmtClause.h"
#include "qps/clauses/SuchThatClause.h"

typedef StmtStmtInvoker FollowsInvoker;

template <FollowsInvoker invoker>
using AbstractFollowsClause = AbstractStmtStmtClause<
    invoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

constexpr FollowsInvoker followsInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                             const StmtRef &leftArg,
                                             const StmtRef &rightArg){
  return pkbQueryHandler->queryFollows(leftArg, rightArg);
};

constexpr FollowsInvoker followsTInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                              const StmtRef &leftArg,
                                              const StmtRef &rightArg){
  return pkbQueryHandler->queryFollowsStar(leftArg, rightArg);
};

class FollowsClause: public AbstractFollowsClause<followsInvoker> {
 public:
  FollowsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};

class FollowsTClause: public AbstractFollowsClause<followsTInvoker> {
 public:
  FollowsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};
