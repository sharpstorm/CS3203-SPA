#pragma once

#include "qps/clauses/such_that/abstract_clauses/AbstractStmtStmtClause.h"

typedef StmtStmtInvoker ParentInvoker;

template <ParentInvoker invoker>
using AbstractParentClause = AbstractStmtStmtClause<
    invoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

constexpr ParentInvoker parentInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                           StmtRef leftArg,
                                           StmtRef rightArg){
  return pkbQueryHandler->queryParent(leftArg, rightArg);
};

constexpr ParentInvoker parentTInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                            StmtRef leftArg,
                                            StmtRef rightArg){
  return pkbQueryHandler->queryParentStar(leftArg, rightArg);
};

typedef AbstractParentClause<parentInvoker> ParentClause;
typedef AbstractParentClause<parentTInvoker> ParentTClause;
