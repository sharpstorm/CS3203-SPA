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

constexpr ParentInvoker parentInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                           const StmtRef &leftArg,
                                           const StmtRef &rightArg){
  return pkbQueryHandler->queryParent(leftArg, rightArg);
};

constexpr ParentInvoker parentTInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                            const StmtRef &leftArg,
                                            const StmtRef &rightArg){
  return pkbQueryHandler->queryParentStar(leftArg, rightArg);
};

constexpr ParentSameSynInvoker parentSymmetricInvoker =
    [](PkbQueryHandler* pkbQueryHandler,
       const StmtRef &arg){
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
