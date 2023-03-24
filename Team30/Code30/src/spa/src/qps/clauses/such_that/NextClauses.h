#pragma once

#include <utility>
#include <vector>
#include <unordered_set>

#include "abstract_clauses/AbstractStmtStmtClause.h"
#include "qps/clauses/SuchThatClause.h"
#include "qps/cfg/CFGQuerierTypes.h"
#include "qps/cfg/cfg_querier/CFGNextQuerier.h"
#include "qps/cfg/cfg_querier/CFGNextTQuerier.h"

typedef StmtStmtInvoker NextInvoker;
typedef StmtInvoker NextSameSynInvoker;

template <NextInvoker invoker, NextSameSynInvoker sameSynInvoker>
using AbstractNextClause = AbstractStmtStmtClause<
    invoker,
    sameSynInvoker,
    ClauseArgument::isStatement,
    ClauseArgument::isStatement>;

constexpr StmtTypePredicate<QueryExecutorAgent> typeChecker =
    [](const QueryExecutorAgent &agent,
       StmtType type,
       StmtValue stmtNumber) -> bool{
      if (type == StmtType::None) {
        return true;
      }
      return agent->getStatementType(stmtNumber) == type;
    };

typedef CFGNextQuerier<QueryExecutorAgent, typeChecker> ConcreteNextQuerier;
typedef CFGNextTQuerier<QueryExecutorAgent, typeChecker> ConcreteNextTQuerier;

template <class T>
constexpr NextInvoker abstractNextInvoker = [](const QueryExecutorAgent &agent,
                                               const StmtRef &leftArg,
                                               const StmtRef &rightArg){
  vector<CFG*> cfgs;
  QueryResult<StmtValue, StmtValue> result{};

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
    return querier.queryArgs(leftArg, rightArg);
  }


  for (auto it = cfgs.begin(); it != cfgs.end(); it++) {
    T querier(*it, agent);
    querier.queryArgs(leftArg, rightArg, &result);
  }
  return result;
};

template <class T>
constexpr NextSameSynInvoker abstractNextSameSynInvoker = [](
    const QueryExecutorAgent &agent, const StmtRef &arg) ->
    unordered_set<StmtValue> {
  vector<CFG*> cfgs = agent->queryCFGs(StmtRef{StmtType::None, 0});
  unordered_set<StmtValue> result;

  for (auto it = cfgs.begin(); it != cfgs.end(); it++) {
    CFG* cfg = *it;
    T querier(cfg, agent);
    int startingStatement = cfg->getStartingStmtNumber();

    for (int i = 0; i < cfg->getNodeCount(); i++) {
      int statement = startingStatement + i;
      if (!typeChecker(agent, arg.getType(), statement)) {
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

constexpr NextInvoker nextInvoker = abstractNextInvoker<ConcreteNextQuerier>;
constexpr NextInvoker nextTInvoker = abstractNextInvoker<ConcreteNextTQuerier>;
constexpr NextSameSynInvoker nextSameSynInvoker =
    abstractNextSameSynInvoker<ConcreteNextQuerier>;
constexpr NextSameSynInvoker nextTSameSynInvoker =
    abstractNextSameSynInvoker<ConcreteNextTQuerier>;

class NextClause: public AbstractNextClause<
    nextInvoker,
    nextSameSynInvoker> {
 public:
  NextClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table) override {
    return computeComplexityScore<
        COMPLEXITY_MODIFIER_NONE,
        COMPLEXITY_MODIFIER_NONE,
        COMPLEXITY_NEXT>(table);
  }
};

class NextTClause: public AbstractNextClause<
    nextTInvoker,
    nextTSameSynInvoker> {
 public:
  NextTClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractStmtStmtClause(std::move(left), std::move(right)) {
  }

  ComplexityScore getComplexityScore(const OverrideTable *table) override {
    return computeComplexityScore<
        COMPLEXITY_NEXT_T,
        COMPLEXITY_NEXT_T + COMPLEXITY_QUERY_TRANSITIVE,
        COMPLEXITY_NEXT_T + COMPLEXITY_QUERY_TRANSITIVE
    >(table);
  }
};
