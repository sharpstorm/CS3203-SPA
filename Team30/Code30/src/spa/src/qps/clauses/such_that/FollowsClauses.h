#pragma once

#include <utility>
#include "qps/clauses/such_that/abstract_clauses/AbstractStmtStmtClause.h"

typedef StmtStmtInvoker FollowsInvoker;

template <FollowsInvoker invoker>
using AbstractFollowsClause = AbstractStmtStmtClause<
    invoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

constexpr FollowsInvoker followsInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                              StmtRef leftArg,
                                              StmtRef rightArg){
  return pkbQueryHandler->queryFollows(leftArg, rightArg);
};

constexpr FollowsInvoker followsTInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                               StmtRef leftArg,
                                               StmtRef rightArg){
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
