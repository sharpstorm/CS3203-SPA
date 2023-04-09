#pragma once

#include <utility>

#include "abstract_clauses/AbstractStmtStmtClause.h"
#include "qps/clauses/SuchThatClause.h"

typedef StmtStmtInvoker FollowsInvoker;
typedef StmtInvoker FollowsSameSynInvoker;

template<FollowsInvoker invoker, FollowsSameSynInvoker sameSynInvoker>
using AbstractFollowsClause = AbstractStmtStmtClause<
    invoker,
    sameSynInvoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

class FollowsClauseInvokers {
 public:
  static constexpr FollowsInvoker
      followsInvoker = [](const QueryExecutorAgent &agent,
                          const StmtRef &leftArg,
                          const StmtRef &rightArg) {
    return agent->queryFollows(leftArg, rightArg);
  };

  static constexpr FollowsInvoker
      followsTInvoker = [](const QueryExecutorAgent &agent,
                           const StmtRef &leftArg,
                           const StmtRef &rightArg) {
    return agent->queryFollowsStar(leftArg, rightArg);
  };

  static constexpr FollowsSameSynInvoker followsSymmetricInvoker =
      [](const QueryExecutorAgent &agent, const StmtRef &arg) {
        return StmtValueSet{};
      };
};

class FollowsClause : public AbstractFollowsClause<
    FollowsClauseInvokers::followsInvoker,
    FollowsClauseInvokers::followsSymmetricInvoker> {
 public:
  FollowsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table)
  const override {
    return computeNoSymmetryComplexityScore(table);
  }
};

class FollowsTClause : public AbstractFollowsClause<
    FollowsClauseInvokers::followsTInvoker,
    FollowsClauseInvokers::followsSymmetricInvoker> {
 public:
  FollowsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table)
  const override {
    return computeNoSymmetryComplexityScore<
        COMPLEXITY_MODIFIER_NONE,
        COMPLEXITY_QUERY_TRANSITIVE,
        COMPLEXITY_QUERY_TRANSITIVE
    >(table);
  }
};
