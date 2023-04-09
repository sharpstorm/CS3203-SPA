#pragma once

#include <utility>

#include "abstract_clauses/AbstractStmtStmtClause.h"
#include "qps/clauses/SuchThatClause.h"

typedef StmtStmtInvoker ParentInvoker;
typedef StmtInvoker ParentSameSynInvoker;

template<ParentInvoker invoker, ParentSameSynInvoker sameSynInvoker>
using AbstractParentClause = AbstractStmtStmtClause<
    invoker,
    sameSynInvoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

class ParentClauseInvokers {
 public:
  static constexpr ParentInvoker
      parentInvoker = [](const QueryExecutorAgent &agent,
                         const StmtRef &leftArg,
                         const StmtRef &rightArg) {
    return agent->queryParent(leftArg, rightArg);
  };

  static constexpr ParentInvoker
      parentTInvoker = [](const QueryExecutorAgent &agent,
                          const StmtRef &leftArg,
                          const StmtRef &rightArg) {
    return agent->queryParentStar(leftArg, rightArg);
  };

  static constexpr ParentSameSynInvoker parentSymmetricInvoker =
      [](const QueryExecutorAgent &agent, const StmtRef &arg) {
        return StmtValueSet{};
      };
};

class ParentClause : public AbstractParentClause<
    ParentClauseInvokers::parentInvoker,
    ParentClauseInvokers::parentSymmetricInvoker> {
 public:
  ParentClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table)
  const override {
    if (isSameSynonym()) {
      return COMPLEXITY_QUERY_CONSTANT;
    }
    return computeNoSymmetryComplexityScore(table);
  }
};

class ParentTClause : public AbstractParentClause<
    ParentClauseInvokers::parentTInvoker,
    ParentClauseInvokers::parentSymmetricInvoker> {
 public:
  ParentTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table)
  const override {
    if (isSameSynonym()) {
      return COMPLEXITY_QUERY_CONSTANT;
    }
    return computeNoSymmetryComplexityScore<
        COMPLEXITY_MODIFIER_NONE,
        COMPLEXITY_QUERY_TRANSITIVE,
        COMPLEXITY_QUERY_TRANSITIVE
    >(table);
  }
};
