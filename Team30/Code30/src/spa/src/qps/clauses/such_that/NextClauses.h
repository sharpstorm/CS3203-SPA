#pragma once

#include <utility>
#include <vector>

#include "abstract_clauses/AbstractStmtStmtClause.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/cfg/CFGQuerierTypes.h"
#include "qps/cfg/cfg_querier/CFGNextQuerier.h"
#include "qps/cfg/cfg_querier/CFGNextTQuerier.h"

typedef StmtStmtInvoker NextInvoker;

template <NextInvoker invoker>
using AbstractNextClause = AbstractStmtStmtClause<
    invoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

constexpr StmtTypePredicate<PkbQueryHandler> typeChecker =
    [](PkbQueryHandler* pkb,
       StmtType type,
       StmtValue stmtNumber) -> bool{
      if (type == StmtType::None) {
        return true;
      }
      return pkb->getStatementType(stmtNumber) == type;
    };

typedef CFGNextQuerier<PkbQueryHandler, typeChecker> ConcreteNextQuerier;
typedef CFGNextTQuerier<PkbQueryHandler, typeChecker> ConcreteNextTQuerier;

template <class T>
constexpr NextInvoker abstractNextInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                               const StmtRef &leftArg,
                                               const StmtRef &rightArg){
  vector<CFG*> cfgs;
  if (leftArg.isKnown()) {
    cfgs = pkbQueryHandler->queryCFGs(leftArg);
  } else {
    cfgs = pkbQueryHandler->queryCFGs(rightArg);
  }

  if (leftArg.isKnown() || rightArg.isKnown()) {
    T querier(cfgs[0], pkbQueryHandler);
    return querier.queryArgs(leftArg, rightArg);
  }

  QueryResult<StmtValue, StmtValue> result{};
  for (auto it = cfgs.begin(); it != cfgs.end(); it++) {
    T querier(*it, pkbQueryHandler);
    querier.queryArgs(leftArg, rightArg, &result);
  }
  return result;
};

constexpr NextInvoker nextInvoker = abstractNextInvoker<ConcreteNextQuerier>;
constexpr NextInvoker nextTInvoker = abstractNextInvoker<ConcreteNextTQuerier>;

class NextClause: public AbstractNextClause<nextInvoker> {
 public:
  NextClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};

class NextTClause: public AbstractNextClause<nextTInvoker> {
 public:
  NextTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};
