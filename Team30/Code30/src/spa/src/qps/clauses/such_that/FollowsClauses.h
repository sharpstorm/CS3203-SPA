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

constexpr FollowsInvoker followsInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                             OverrideTable* table,
                                             const StmtRef &leftArg,
                                             const StmtRef &rightArg){
  return pkbQueryHandler->queryFollows(leftArg, rightArg);
};

constexpr FollowsInvoker followsTInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                              OverrideTable* table,
                                              const StmtRef &leftArg,
                                              const StmtRef &rightArg){
  return pkbQueryHandler->queryFollowsStar(leftArg, rightArg);
};

constexpr FollowsSameSynInvoker followsSymmetricInvoker =
    [](PkbQueryHandler* pkbQueryHandler,
       OverrideTable* table,
       const StmtRef &arg){
      return unordered_set<StmtValue>{};
    };

class FollowsClause: public AbstractFollowsClause<
    followsInvoker,
    followsSymmetricInvoker> {
 public:
  FollowsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};

class FollowsTClause: public AbstractFollowsClause<
    followsTInvoker,
    followsSymmetricInvoker> {
 public:
  FollowsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};
