#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "abstract_clauses/AbstractStmtStmtClause.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/cfg/CFGQuerierTypes.h"
#include "qps/cfg/cfg_querier/CFGNextQuerier.h"
#include "qps/cfg/cfg_querier/CFGNextTQuerier.h"

using std::make_unique;

typedef StmtStmtInvoker NextInvoker;
typedef StmtInvoker NextSameSynInvoker;

template<NextInvoker invoker, NextSameSynInvoker sameSynInvoker>
using AbstractNextClause = AbstractStmtStmtClause<
    invoker,
    sameSynInvoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

class NextClauseInvokers {
  static constexpr StmtTypePredicate<QueryExecutorAgent> typeChecker =
      [](const QueryExecutorAgent &agent,
         StmtType type,
         StmtValue stmtNumber) -> bool {
        if (type == StmtType::None || type == StmtType::Wildcard) {
          return true;
        }
        return agent->getStatementType(stmtNumber) == type;
      };

  typedef CFGNextQuerier<QueryExecutorAgent, typeChecker> ConcreteNextQuerier;
  typedef CFGNextTQuerier<QueryExecutorAgent, typeChecker> ConcreteNextTQuerier;

  template<class T>
  static constexpr NextInvoker
      abstractNextInvoker = [](const QueryExecutorAgent &agent,
                               const StmtRef &leftArg,
                               const StmtRef &rightArg) {
    vector<CFG *> cfgs;
    auto result = make_unique<QueryResult<StmtValue, StmtValue>>();

    if (leftArg.isKnown()) {
      cfgs = agent->queryCFGs(leftArg);
    } else {
      cfgs = agent->queryCFGs(rightArg);
    }

    if (cfgs.empty()) {
      return result;
    }

    if (leftArg.isKnown() || rightArg.isKnown()) {
      T querier(cfgs[0], agent);
      querier.queryArgs(leftArg, rightArg, result.get());
      return result;
    }

    for (auto it = cfgs.begin(); it != cfgs.end(); it++) {
      T querier(*it, agent);
      querier.queryArgs(leftArg, rightArg, result.get());
    }
    return result;
  };

  template<class T>
  static const void queryBoolCFG(const QueryExecutorAgent &agent,
                                 CFG *cfg,
                                 StmtValueSet *result,
                                 StmtType type) {
    T querier(cfg, agent);
    for (int i = 0; i < cfg->getNodeCount(); i++) {
      StmtValue statement = cfg->fromCFGNode(i);
      if (!typeChecker(agent, type, statement)) {
        continue;
      }

      StmtTransitiveResult relationResult;
      querier.queryBool(&relationResult, statement, statement);
      if (!relationResult.empty()) {
        result->insert(statement);
      }
    }
  }

  template<class T>
  static constexpr NextSameSynInvoker abstractNextSameSynInvoker = [](
      const QueryExecutorAgent &agent, const StmtRef &arg) -> StmtValueSet {
    vector<CFG *> cfgs = agent->queryCFGs(StmtRef{StmtType::None, 0});
    StmtValueSet result;

    for (CFG *cfg : cfgs) {
      queryBoolCFG<T>(agent, cfg, &result, arg.getType());
    }
    return result;
  };

 public:
  static constexpr NextInvoker
      nextInvoker = abstractNextInvoker<ConcreteNextQuerier>;
  static constexpr NextInvoker
      nextTInvoker = abstractNextInvoker<ConcreteNextTQuerier>;
  static constexpr NextSameSynInvoker nextSameSynInvoker =
      abstractNextSameSynInvoker<ConcreteNextQuerier>;
  static constexpr NextSameSynInvoker nextTSameSynInvoker =
      abstractNextSameSynInvoker<ConcreteNextTQuerier>;
};

class NextClause : public AbstractNextClause<
    NextClauseInvokers::nextInvoker,
    NextClauseInvokers::nextSameSynInvoker> {
 public:
  NextClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table)
  const override {
    return computeNoSymmetryComplexityScore<
        COMPLEXITY_MODIFIER_NONE,
        COMPLEXITY_MODIFIER_NONE,
        COMPLEXITY_NEXT>(table);
  }
};

class NextTClause : public AbstractNextClause<
    NextClauseInvokers::nextTInvoker,
    NextClauseInvokers::nextTSameSynInvoker> {
 public:
  NextTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table)
  const override {
    return computeComplexityScore<
        COMPLEXITY_NEXT_T_CONST,
        COMPLEXITY_NEXT_T + COMPLEXITY_QUERY_TRANSITIVE,
        COMPLEXITY_NEXT_T + COMPLEXITY_QUERY_TRANSITIVE
    >(table);
  }
};
