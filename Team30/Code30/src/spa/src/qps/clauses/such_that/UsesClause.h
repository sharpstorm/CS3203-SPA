#pragma once

#include <utility>

#include "abstract_clauses/AbstractAnyEntClause.h"
#include "qps/clauses/SuchThatClause.h"

typedef StmtEntInvoker UsesSInvoker;
typedef EntEntInvoker UsesPInvoker;

class UsesClauseInvokers {
 public:
  static constexpr UsesSInvoker
      usesSInvoker = [](const QueryExecutorAgent &agent,
                        const StmtRef &leftArg,
                        const EntityRef &rightArg) {
    return agent->queryUses(leftArg, rightArg);
  };

  static constexpr UsesPInvoker
      usesPInvoker = [](const QueryExecutorAgent &agent,
                        const EntityRef &leftArg,
                        const EntityRef &rightArg) {
    return agent->queryUses(leftArg, rightArg);
  };
};

typedef AbstractAnyEntClause<
    UsesClauseInvokers::usesPInvoker,
    UsesClauseInvokers::usesSInvoker,
    ClauseArgument::isEither<ClauseArgument::isStatement,
                             ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>>,
    ClauseArgument::isType<PQL_SYN_TYPE_VARIABLE>> AbstractUsesClause;

class UsesClause : public AbstractUsesClause {
 public:
  UsesClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractAnyEntClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table)
  const override {
    return computeComplexityScore(table);
  }
};
