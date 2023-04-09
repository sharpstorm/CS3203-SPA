#pragma once

#include <utility>

#include "abstract_clauses/AbstractAnyEntClause.h"
#include "qps/clauses/SuchThatClause.h"

typedef StmtEntInvoker ModifiesSInvoker;
typedef EntEntInvoker ModifiesPInvoker;

class ModifiesClauseInvokers {
 public:
  static constexpr ModifiesSInvoker modifiesSInvoker = [](
      const QueryExecutorAgent &agent,
      const StmtRef &leftArg,
      const EntityRef &rightArg) {
    return agent->queryModifies(leftArg, rightArg);
  };

  static constexpr ModifiesPInvoker modifiesPInvoker = [](
      const QueryExecutorAgent &agent,
      const EntityRef &leftArg,
      const EntityRef &rightArg) {
    return agent->queryModifies(leftArg, rightArg);
  };
};

typedef AbstractAnyEntClause<
    ModifiesClauseInvokers::modifiesPInvoker,
    ModifiesClauseInvokers::modifiesSInvoker,
    ClauseArgument::isEither<ClauseArgument::isStatement,
                             ClauseArgument::isType<PQL_SYN_TYPE_PROCEDURE>>,
    ClauseArgument::isType<PQL_SYN_TYPE_VARIABLE>> AbstractModifiesClause;

class ModifiesClause : public AbstractModifiesClause {
 public:
  ModifiesClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractAnyEntClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table)
  const override {
    return computeComplexityScore(table);
  }
};
