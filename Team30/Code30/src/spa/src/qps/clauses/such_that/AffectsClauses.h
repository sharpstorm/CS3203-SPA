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

class AffectsClauseInvokers {
  static constexpr StmtTypePredicate<QueryExecutorAgent> typeChecker =
      [](const QueryExecutorAgent &agent,
         StmtType type,
         StmtValue stmtNumber) -> bool {
        if (type == StmtType::None || type == StmtType::Wildcard) {
          return true;
        }
        return agent->getStatementType(stmtNumber) == type;
      };

  static constexpr ModifiesGetter<QueryExecutorAgent> modifiesQuerier =
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

  static constexpr UsesGetter<QueryExecutorAgent> usesQuerier =
      [](const QueryExecutorAgent &agent,
         StmtValue stmtNumber) -> EntityIdxSet {
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
                            modifiesQuerier, usesQuerier>
      ConcreteAffectsQuerier;

  typedef CFGAffectsTQuerier<ConcreteAffectsQuerier>
      ConcreteAffectsTQuerier;

  static constexpr auto validateAffectsType = [](const StmtRef &ref) {
    return ref.isAnyType(StmtType::None,
                         StmtType::Assign,
                         StmtType::Wildcard);
  };

  template<class Querier>
  using QuerierFactory = Querier(*)(CFG *cfg, const QueryExecutorAgent &agent);

  template<class QuerierT, QuerierFactory<QuerierT> makeQuerier>
  static constexpr AffectsInvoker abstractAffectsInvoker = [](
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
      QuerierT querier = makeQuerier(cfgs[0], agent);
      return make_unique<QueryResult<StmtValue, StmtValue>>(
          querier.queryArgs(leftArg, rightArg));
    }

    for (auto it = cfgs.begin(); it != cfgs.end(); it++) {
      QuerierT querier = makeQuerier(*it, agent);
      querier.queryArgs(leftArg, rightArg, result.get());
    }
    return result;
  };

  template<class QuerierT, QuerierFactory<QuerierT> makeQuerier>
  static constexpr AffectsSameSynInvoker abstractAffectsSymmetricInvoker =
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
          QuerierT querier = makeQuerier(cfg, agent);
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

  static constexpr QuerierFactory<ConcreteAffectsQuerier>
      affectsQuerierFactory = [](CFG *cfg, const QueryExecutorAgent &agent) {
    return ConcreteAffectsQuerier(cfg, agent);
  };

  static constexpr QuerierFactory<ConcreteAffectsTQuerier>
      affectsTQuerierFactory = [](CFG *cfg, const QueryExecutorAgent &agent) {
    return ConcreteAffectsTQuerier(cfg, ConcreteAffectsQuerier(cfg, agent));
  };

 public:
  static constexpr AffectsInvoker affectsInvoker =
      abstractAffectsInvoker<ConcreteAffectsQuerier, affectsQuerierFactory>;
  static constexpr AffectsInvoker affectsTInvoker =
      abstractAffectsInvoker<ConcreteAffectsTQuerier, affectsTQuerierFactory>;
  static constexpr AffectsSameSynInvoker affectsSymmetricInvoker =
      abstractAffectsSymmetricInvoker<ConcreteAffectsQuerier, affectsQuerierFactory>;
  static constexpr AffectsSameSynInvoker affectsTSymmetricInvoker =
      abstractAffectsSymmetricInvoker<ConcreteAffectsTQuerier, affectsTQuerierFactory>;
};

class AffectsClause : public AbstractAffectsClause<
    AffectsClauseInvokers::affectsInvoker,
    AffectsClauseInvokers::affectsSymmetricInvoker> {
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
    AffectsClauseInvokers::affectsTInvoker,
    AffectsClauseInvokers::affectsTSymmetricInvoker> {
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
