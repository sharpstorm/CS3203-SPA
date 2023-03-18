#pragma once

#include <utility>
#include <unordered_set>

#include "abstract_clauses/AbstractStmtStmtClause.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/cfg/cfg_querier/CFGAffectsQuerier.h"

typedef StmtStmtInvoker AffectsInvoker;
typedef StmtInvoker AffectsSameSynInvoker;

template <AffectsInvoker invoker, AffectsSameSynInvoker sameSynInvoker>
using AbstractAffectsClause = AbstractStmtStmtClause<
    invoker,
    sameSynInvoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

constexpr ModifiesGetter<PkbQueryHandler> modifiesQuerier =
    [](PkbQueryHandler* pkb,
       StmtValue stmtNumber) -> EntityValue {
      QueryResult<StmtValue, EntityValue> result =
          pkb->queryModifies(StmtRef{StmtType::None, stmtNumber},
                             EntityRef{EntityType::None, ""});
      if (result.isEmpty) {
        return "";
      }
      for (auto it : result.secondArgVals) {
        return it;
      }

      return "";
    };

constexpr UsesGetter<PkbQueryHandler> usesQuerier =
    [](PkbQueryHandler* pkb,
       StmtValue stmtNumber) -> unordered_set<EntityValue> {
      QueryResult<StmtValue, EntityValue> result =
          pkb->queryUses(StmtRef{StmtType::None, stmtNumber},
                         EntityRef{EntityType::None, ""});
      return result.secondArgVals;
    };


typedef CFGAffectsQuerier<PkbQueryHandler, typeChecker,
                          modifiesQuerier, usesQuerier> ConcreteAffectsQuerier;

constexpr AffectsInvoker affectsInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                             const StmtRef &leftArg,
                                             const StmtRef &rightArg){
  QueryResult<StmtValue, StmtValue> result{};
  if (leftArg.type != StmtType::None && leftArg.type != StmtType::Assign) {
    return result;
  }
  if (rightArg.type != StmtType::None && rightArg.type != StmtType::Assign) {
    return result;
  }

  vector<CFG*> cfgs;
  if (leftArg.isKnown()) {
    cfgs = pkbQueryHandler->queryCFGs(leftArg);
  } else {
    cfgs = pkbQueryHandler->queryCFGs(rightArg);
  }

  if (leftArg.isKnown() || rightArg.isKnown()) {
    ConcreteAffectsQuerier querier(cfgs[0], pkbQueryHandler);
    return querier.queryArgs(leftArg, rightArg);
  }

  for (auto it = cfgs.begin(); it != cfgs.end(); it++) {
    ConcreteAffectsQuerier querier(*it, pkbQueryHandler);
    querier.queryArgs(leftArg, rightArg, &result);
  }
  return result;
};

constexpr AffectsInvoker affectsTInvoker = [](PkbQueryHandler* pkbQueryHandler,
                                              const StmtRef &leftArg,
                                              const StmtRef &rightArg){
  return QueryResult<StmtValue, StmtValue>{};
};

constexpr AffectsSameSynInvoker affectsSymmetricInvoker =
    [](PkbQueryHandler* pkbQueryHandler,
       const StmtRef &arg){
      vector<CFG*> cfgs = pkbQueryHandler->queryCFGs(StmtRef{StmtType::None, 0});
      unordered_set<StmtValue> result;

      for (auto it = cfgs.begin(); it != cfgs.end(); it++) {
        CFG* cfg = *it;
        ConcreteAffectsQuerier querier(cfg, pkbQueryHandler);
        int startingStatement = cfg->getStartingStmtNumber();

        for (int i = 0; i < cfg->getNodeCount(); i++) {
          int statement = startingStatement + i;
          if (!typeChecker(pkbQueryHandler, StmtType::Assign, statement)) {
            continue;
          }

          auto relationResult = querier.queryBool(statement, statement);
          if (!relationResult.isEmpty) {
            result.insert(statement);
          }
        }
      }

      return result;
    };

class AffectsClause: public AbstractAffectsClause<
    affectsInvoker,
    affectsSymmetricInvoker> {
 public:
  AffectsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};

class AffectsTClause: public AbstractAffectsClause<
    affectsTInvoker,
    affectsSymmetricInvoker> {
 public:
  AffectsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }
};
