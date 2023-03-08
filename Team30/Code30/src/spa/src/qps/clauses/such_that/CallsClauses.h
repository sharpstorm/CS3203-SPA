#pragma once

#include "qps/clauses/such_that/abstract_clauses/AbstractEntEntClause.h"

typedef EntEntInvoker CallsInvoker;

template <CallsInvoker invoker>
using AbstractCallsClause = AbstractEntEntClause<
    invoker,
    ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>,
    ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>>;

constexpr CallsInvoker callsInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                          EntityRef leftArg,
                                          EntityRef rightArg){
  return pkbQueryHandler->queryCalls(leftArg, rightArg);
};

constexpr CallsInvoker callsTInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                           EntityRef leftArg,
                                           EntityRef rightArg){
  return pkbQueryHandler->queryCallsStar(leftArg, rightArg);
};

typedef AbstractCallsClause<callsInvoker> CallsClause;
typedef AbstractCallsClause<callsTInvoker> CallsTClause;
