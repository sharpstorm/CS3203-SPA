#pragma once

#include <utility>

#include "abstract_clauses/AbstractEntEntClause.h"
#include "qps/clauses/SuchThatClause.h"

typedef EntEntInvoker CallsInvoker;
typedef EntInvoker CallsSameSynInvoker;

template<CallsInvoker invoker, CallsSameSynInvoker symmetricInvoker>
using AbstractCallsClause = AbstractEntEntClause<
    invoker,
    symmetricInvoker,
    ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>,
    ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>>;

class CallsClauseInvokers {
 public:
  static constexpr CallsInvoker
      callsInvoker = [](const QueryExecutorAgent &agent,
                        const EntityRef &leftArg,
                        const EntityRef &rightArg) {
    return agent->queryCalls(leftArg, rightArg);
  };

  static constexpr CallsInvoker
      callsTInvoker = [](const QueryExecutorAgent &agent,
                         const EntityRef &leftArg,
                         const EntityRef &rightArg) {
    return agent->queryCallsStar(leftArg, rightArg);
  };

  static constexpr CallsSameSynInvoker
      callsSymmetricInvoker =
      [](const QueryExecutorAgent &agent, const EntityRef &arg) {
        return EntityValueSet{};
      };
};

class CallsClause : public AbstractCallsClause<
    CallsClauseInvokers::callsInvoker,
    CallsClauseInvokers::callsSymmetricInvoker> {
 public:
  CallsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractEntEntClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table)
  const override {
    return computeNoSymmetryComplexityScore<
        COMPLEXITY_MODIFIER_PREFERRED,
        COMPLEXITY_MODIFIER_NONE,
        COMPLEXITY_MODIFIER_NONE>(table);
  }
};

class CallsTClause : public AbstractCallsClause<
    CallsClauseInvokers::callsTInvoker,
    CallsClauseInvokers::callsSymmetricInvoker> {
 public:
  CallsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractEntEntClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table)
  const override {
    return computeNoSymmetryComplexityScore<
        COMPLEXITY_MODIFIER_PREFERRED,
        COMPLEXITY_QUERY_TRANSITIVE,
        COMPLEXITY_QUERY_TRANSITIVE>(table);
  }
};
