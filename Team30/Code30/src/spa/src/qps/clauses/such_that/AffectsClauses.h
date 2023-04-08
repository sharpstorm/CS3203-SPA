#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "abstract_clauses/AbstractStmtStmtClause.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/cfg/cfg_querier/CFGAffectsQuerier.h"
#include "qps/cfg/cfg_querier/CFGAffectsTQuerier.h"

using std::unique_ptr, std::make_unique;

typedef StmtStmtInvoker AffectsInvoker;
typedef StmtInvoker AffectsSameSynInvoker;

template<AffectsInvoker invoker, AffectsSameSynInvoker sameSynInvoker>
using AbstractAffectsClause = AbstractStmtStmtClause<
    invoker,
    sameSynInvoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

constexpr ModifiesGetter<QueryExecutorAgent> modifiesQuerier =
    [](const QueryExecutorAgent &agent,
       StmtValue stmtNumber) -> EntityIdxSet {
      QueryResultPtr<StmtValue, EntityValue> result =
          agent->queryModifies(StmtRef{StmtType::None, stmtNumber},
                               EntityRef{EntityType::Variable, ""});
      EntityIdxSet ret;
      for (const EntityValue &v : result->secondArgVals) {
        ret.insert(agent->getIndexOfVariable(v));
      }
      ret.erase(NO_ENT_INDEX);
      return ret;
    };

constexpr UsesGetter<QueryExecutorAgent> usesQuerier =
    [](const QueryExecutorAgent &agent, StmtValue stmtNumber) -> EntityIdxSet {
      QueryResultPtr<StmtValue, EntityValue> result =
          agent->queryUses(StmtRef{StmtType::None, stmtNumber},
                           EntityRef{EntityType::Variable, ""});
      EntityIdxSet ret;
      for (const EntityValue &v : result->secondArgVals) {
        ret.insert(agent->getIndexOfVariable(v));
      }
      ret.erase(NO_ENT_INDEX);
      return ret;
    };

typedef CFGAffectsQuerier<QueryExecutorAgent, typeChecker,
                          modifiesQuerier, usesQuerier> ConcreteAffectsQuerier;

typedef CFGAffectsTQuerier<QueryExecutorAgent, typeChecker,
                           modifiesQuerier, usesQuerier>
    ConcreteAffectsTQuerier;

constexpr auto validateAffectsType = [](const StmtRef &ref) {
  return ref.isAnyType(StmtType::None,
                       StmtType::Assign,
                       StmtType::Wildcard);
};

template<class QuerierT>
constexpr AffectsInvoker abstractAffectsInvoker = [](
    const QueryExecutorAgent &agent,
    const StmtRef &leftArg,
    const StmtRef &rightArg) {
  auto result = make_unique<QueryResult<StmtValue, StmtValue>>();
  if (!validateAffectsType(leftArg)) {
    return result;
  }
  if (!validateAffectsType(rightArg)) {
    return result;
  }

  vector<CFG *> cfgs;
  if (leftArg.isKnown()) {
    cfgs = agent->queryCFGs(leftArg);
  } else {
    cfgs = agent->queryCFGs(rightArg);
  }

  if (cfgs.empty()) {
    return result;
  }

  if (leftArg.isKnown() || rightArg.isKnown()) {
    QuerierT querier(cfgs[0], agent);
    return make_unique<QueryResult<StmtValue, StmtValue>>(
        querier.queryArgs(leftArg, rightArg));
  }

  for (auto it = cfgs.begin(); it != cfgs.end(); it++) {
    QuerierT querier(*it, agent);
    querier.queryArgs(leftArg, rightArg, result.get());
  }
  return result;
};

template<class QuerierT>
constexpr AffectsSameSynInvoker abstractAffectsSymmetricInvoker =
    [](const QueryExecutorAgent &agent,
       const StmtRef &arg) {
      StmtValueSet result;

      if (!arg.isType(StmtType::None) && !arg.isType(StmtType::Assign)) {
        return result;
      }

      vector<CFG *> cfgs = agent->queryCFGs(
          StmtRef{StmtType::None, 0});

      for (auto it = cfgs.begin(); it != cfgs.end(); it++) {
        CFG *cfg = *it;
        QuerierT querier(cfg, agent);
        StmtValue startingStatement = cfg->getStartingStmtNumber();

        for (int i = 0; i < cfg->getNodeCount(); i++) {
          StmtValue statement = startingStatement + i;
          if (!typeChecker(agent, StmtType::Assign, statement)) {
            continue;
          }

          auto relationResult = querier.queryBool(statement, statement);
          if (!relationResult.empty()) {
            result.insert(statement);
          }
        }
      }

      return result;
    };

constexpr AffectsInvoker affectsInvoker =
    abstractAffectsInvoker<ConcreteAffectsQuerier>;
constexpr AffectsInvoker affectsTInvoker =
    abstractAffectsInvoker<ConcreteAffectsTQuerier>;
constexpr AffectsSameSynInvoker affectsSymmetricInvoker =
    abstractAffectsSymmetricInvoker<ConcreteAffectsQuerier>;
constexpr AffectsSameSynInvoker affectsTSymmetricInvoker =
    abstractAffectsSymmetricInvoker<ConcreteAffectsTQuerier>;

class AffectsClause : public AbstractAffectsClause<
    affectsInvoker,
    affectsSymmetricInvoker> {
 public:
  AffectsClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table)
  const override {
    return computeComplexityScore<
        COMPLEXITY_AFFECTS_CONST,
        COMPLEXITY_AFFECTS,
        COMPLEXITY_AFFECTS>(table);
  }
};

class AffectsTClause : public AbstractAffectsClause<
    affectsTInvoker,
    affectsTSymmetricInvoker> {
 public:
  AffectsTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table)
  const override {
    return computeComplexityScore<
        COMPLEXITY_AFFECTS_T_CONST,
        COMPLEXITY_AFFECTS_T + COMPLEXITY_QUERY_TRANSITIVE,
        COMPLEXITY_AFFECTS_T + COMPLEXITY_QUERY_TRANSITIVE
    >(table);
  }
};
