#pragma once

#include <utility>

#include "abstract_clauses/AbstractEntEntClause.h"
#include "qps/clauses/SuchThatClause.h"

typedef EntEntInvoker CallsInvoker;
typedef EntInvoker CallsSameSynInvoker;

template <CallsInvoker invoker, CallsSameSynInvoker symmetricInvoker>
using AbstractCallsClause = AbstractEntEntClause<
    invoker,
    symmetricInvoker,
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

constexpr CallsSameSynInvoker callsSymmetricInvoker =
    [](PkbQueryHandler* pkbQueryHandler,
       const EntityRef &arg){
      return unordered_set<EntityValue>{};
    };

class CallsClause: public AbstractCallsClause<
    callsInvoker,
    callsSymmetricInvoker> {
 public:
  CallsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractEntEntClause(std::move(left), std::move(right)) {
  }
};

class CallsTClause: public AbstractCallsClause<
    callsTInvoker,
    callsSymmetricInvoker> {
 public:
  CallsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractEntEntClause(std::move(left), std::move(right)) {
  }
};
