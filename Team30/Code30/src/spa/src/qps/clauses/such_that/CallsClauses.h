#pragma once

#include <utility>

#include "qps/clauses/abstract_clauses/AbstractEntEntClause.h"

typedef EntEntInvoker CallsInvoker;

template <CallsInvoker invoker>
using AbstractCallsClause = AbstractEntEntClause<
    invoker,
    ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>,
    ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>>;

constexpr CallsInvoker callsInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                         const EntityRef &leftArg,
                                         const EntityRef &rightArg){
  return pkbQueryHandler->queryCalls(leftArg, rightArg);
};

constexpr CallsInvoker callsTInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                          const EntityRef &leftArg,
                                          const EntityRef &rightArg){
  return pkbQueryHandler->queryCallsStar(leftArg, rightArg);
};

class CallsClause: public AbstractCallsClause<callsInvoker> {
 public:
  CallsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractEntEntClause(std::move(left), std::move(right)) {
  }
};

class CallsTClause: public AbstractCallsClause<callsTInvoker> {
 public:
  CallsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractEntEntClause(std::move(left), std::move(right)) {
  }
};

