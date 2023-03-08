#pragma once

#include "qps/clauses/such_that/abstract_clauses/AbstractStmtStmtClause.h"
#include "abstract_clauses/AbstractSuchThatClause.h"

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

typedef AbstractFollowsClause<followsInvoker> FollowsClause;
typedef AbstractFollowsClause<followsTInvoker> FollowsTClause;
