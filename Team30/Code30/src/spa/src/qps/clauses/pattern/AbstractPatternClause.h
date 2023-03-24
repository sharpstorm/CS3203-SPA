#pragma once

#include <string>
#include <utility>

#include "qps/common/PQLTypes.h"
#include "common/Types.h"
#include "qps/clauses/PatternClause.h"
#include "qps/clauses/InvokerTypes.h"
#include "qps/clauses/ClauseScoring.h"

using PatternQueryInvoker = QueryInvoker<StmtValue, StmtRef,
                                         EntityValue, EntityRef>;

template <PQLSynonymType SYN_TYPE, StmtType StatementType,
    PatternQueryInvoker invoker>
class AbstractPatternClause: public PatternClause {
 protected:
  AbstractPatternClause(const PQLQuerySynonymProxy &synonym,
                        ClauseArgumentPtr leftArg):
      PatternClause(synonym, std::move(leftArg), SYN_TYPE) {
  }

  PQLQueryResult* evaluateOn(const QueryExecutorAgent &agent) {
    StmtRef leftStatement = {StatementType, 0};
    EntityRef leftVar = leftArg->toEntityRef();
    leftStatement = agent.transformArg(synonym->getName(), leftStatement);
    leftVar = agent.transformArg(leftArg->getName(), leftVar);

    if (!agent.isValid(leftStatement) || !agent.isValid(leftVar)) {
      return new PQLQueryResult();
    }

    QueryResult<StmtValue, EntityValue> result =
        invoker(agent, leftStatement, leftVar);
    return Clause::toQueryResult(synonym->getName(), leftArg.get(), result);
  }

 public:
  ComplexityScore getComplexityScore(const OverrideTable *table) override {
    if (table->contains(leftArg->getName())) {
      return COMPLEXITY_QUERY_CONSTANT;
    }
    return COMPLEXITY_QUERY_SYN_CONTAINER;
  }
};
