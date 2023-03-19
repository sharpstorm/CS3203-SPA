#pragma once

#include <utility>
#include <unordered_set>

#include "abstract_clauses/AbstractStmtStmtClause.h"
#include "qps/clauses/SuchThatClause.h"

typedef StmtStmtInvoker ParentInvoker;
typedef StmtInvoker ParentSameSynInvoker;

template <ParentInvoker invoker, ParentSameSynInvoker sameSynInvoker>
using AbstractParentClause = AbstractStmtStmtClause<
    invoker,
    sameSynInvoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

constexpr ParentInvoker parentInvoker = [](const QueryExecutorAgent &agent,
                                           const StmtRef &leftArg,
                                           const StmtRef &rightArg){
  return agent->queryParent(leftArg, rightArg);
};

constexpr ParentInvoker parentTInvoker = [](const QueryExecutorAgent &agent,
                                            const StmtRef &leftArg,
                                            const StmtRef &rightArg){
  return agent->queryParentStar(leftArg, rightArg);
};

constexpr ParentSameSynInvoker parentSymmetricInvoker =
    [](const QueryExecutorAgent &agent, const StmtRef &arg){
      return unordered_set<StmtValue>{};
    };

class ParentClause: public AbstractParentClause<parentInvoker,
                                                parentSymmetricInvoker> {
 public:
  ParentClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};

class ParentTClause: public AbstractParentClause<parentTInvoker,
                                                 parentSymmetricInvoker> {
 public:
  ParentTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};
