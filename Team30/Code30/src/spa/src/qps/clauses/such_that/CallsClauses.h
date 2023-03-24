#pragma once

#include <utility>
#include <unordered_set>

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

constexpr CallsInvoker callsInvoker = [](const QueryExecutorAgent &agent,
                                         const EntityRef &leftArg,
                                         const EntityRef &rightArg){
  return agent->queryCalls(leftArg, rightArg);
};

constexpr CallsInvoker callsTInvoker = [](const QueryExecutorAgent &agent,
                                          const EntityRef &leftArg,
                                          const EntityRef &rightArg){
  return agent->queryCallsStar(leftArg, rightArg);
};

constexpr CallsSameSynInvoker callsSymmetricInvoker =
    [](const QueryExecutorAgent &agent, const EntityRef &arg){
      return unordered_set<EntityValue>{};
    };

class CallsClause: public AbstractCallsClause<
    callsInvoker,
    callsSymmetricInvoker> {
 public:
  CallsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractEntEntClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable &table) override {
    return computeComplexityScore(table);
  }
};

class CallsTClause: public AbstractCallsClause<
    callsTInvoker,
    callsSymmetricInvoker> {
 public:
  CallsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractEntEntClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable &table) override {
    return computeComplexityScore<
        COMPLEXITY_MODIFIER_NONE,
        COMPLEXITY_QUERY_TRANSITIVE,
        COMPLEXITY_QUERY_TRANSITIVE
    >(table);
  }
};
