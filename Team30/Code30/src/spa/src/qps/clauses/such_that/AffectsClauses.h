#pragma once

#include <utility>
#include <unordered_set>
#include <vector>

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

constexpr ModifiesGetter<QueryExecutorAgent> modifiesQuerier =
    [](const QueryExecutorAgent &agent,
       StmtValue stmtNumber) -> EntityValue {
      QueryResult<StmtValue, EntityValue> result =
          agent->queryModifies(StmtRef{StmtType::None, stmtNumber},
                               EntityRef{EntityType::None, ""});
      if (result.isEmpty) {
        return "";
      }
      for (auto it : result.secondArgVals) {
        return it;
      }

      return "";
    };

constexpr UsesGetter<QueryExecutorAgent> usesQuerier =
    [](const QueryExecutorAgent &agent,
       StmtValue stmtNumber) -> unordered_set<EntityValue> {
      QueryResult<StmtValue, EntityValue> result =
          agent->queryUses(StmtRef{StmtType::None, stmtNumber},
                           EntityRef{EntityType::None, ""});
      return result.secondArgVals;
    };


typedef CFGAffectsQuerier<QueryExecutorAgent, typeChecker,
                          modifiesQuerier, usesQuerier> ConcreteAffectsQuerier;

constexpr AffectsInvoker affectsInvoker = [](const QueryExecutorAgent &agent,
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
    cfgs = agent->queryCFGs(leftArg);
  } else {
    cfgs = agent->queryCFGs(rightArg);
  }

  if (cfgs.empty()) {
    return result;
  }

  if (leftArg.isKnown() || rightArg.isKnown()) {
    ConcreteAffectsQuerier querier(cfgs[0], agent);
    return querier.queryArgs(leftArg, rightArg);
  }

  for (auto it = cfgs.begin(); it != cfgs.end(); it++) {
    ConcreteAffectsQuerier querier(*it, agent);
    querier.queryArgs(leftArg, rightArg, &result);
  }
  return result;
};

constexpr AffectsInvoker affectsTInvoker = [](const QueryExecutorAgent &agent,
                                              const StmtRef &leftArg,
                                              const StmtRef &rightArg){
  return QueryResult<StmtValue, StmtValue>{};
};

constexpr AffectsSameSynInvoker affectsSymmetricInvoker =
    [](const QueryExecutorAgent &agent,
       const StmtRef &arg){
      unordered_set<StmtValue> result;

      if (arg.type != StmtType::None && arg.type != StmtType::Assign) {
        return result;
      }

      vector<CFG*> cfgs = agent->queryCFGs(
          StmtRef{StmtType::None, 0});

      for (auto it = cfgs.begin(); it != cfgs.end(); it++) {
        CFG* cfg = *it;
        ConcreteAffectsQuerier querier(cfg, agent);
        int startingStatement = cfg->getStartingStmtNumber();

        for (int i = 0; i < cfg->getNodeCount(); i++) {
          int statement = startingStatement + i;
          if (!typeChecker(agent, StmtType::Assign, statement)) {
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
