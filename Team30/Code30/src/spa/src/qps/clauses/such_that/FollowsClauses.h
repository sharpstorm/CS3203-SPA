#pragma once

#include <utility>
#include <unordered_set>

#include "abstract_clauses/AbstractStmtStmtClause.h"
#include "qps/clauses/SuchThatClause.h"

typedef StmtStmtInvoker FollowsInvoker;
typedef StmtInvoker FollowsSameSynInvoker;

template <FollowsInvoker invoker, FollowsSameSynInvoker sameSynInvoker>
using AbstractFollowsClause = AbstractStmtStmtClause<
    invoker,
    sameSynInvoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

constexpr FollowsInvoker followsInvoker = [](const QueryExecutorAgent &agent,
                                             const StmtRef &leftArg,
                                             const StmtRef &rightArg) {
  return agent->queryFollows(leftArg, rightArg);
};

constexpr FollowsInvoker followsTInvoker = [](const QueryExecutorAgent &agent,
                                              const StmtRef &leftArg,
                                              const StmtRef &rightArg){
  return agent->queryFollowsStar(leftArg, rightArg);
};

constexpr FollowsSameSynInvoker followsSymmetricInvoker =
    [](const QueryExecutorAgent &agent, const StmtRef &arg){
      return unordered_set<StmtValue>{};
    };

class FollowsClause: public AbstractFollowsClause<
    followsInvoker,
    followsSymmetricInvoker> {
 public:
  FollowsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table) override {
    return computeComplexityScore(table);
  }
};

class FollowsTClause: public AbstractFollowsClause<
    followsTInvoker,
    followsSymmetricInvoker> {
 public:
  FollowsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table) override {
    return computeComplexityScore<
        COMPLEXITY_MODIFIER_NONE,
        COMPLEXITY_QUERY_TRANSITIVE,
        COMPLEXITY_QUERY_TRANSITIVE
    >(table);
  }
};
