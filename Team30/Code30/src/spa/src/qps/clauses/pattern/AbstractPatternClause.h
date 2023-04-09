#pragma once

#include <memory>
#include <utility>

#include "qps/common/synonym/PQLTypes.h"
#include "common/Types.h"
#include "qps/clauses/PatternClause.h"
#include "qps/clauses/InvokerTypes.h"
#include "qps/clauses/ClauseScoring.h"
#include "qps/common/intermediate_result/PQLQueryResultBuilder.h"

using std::unique_ptr;

using PatternQueryInvoker = QueryInvoker<StmtValue, StmtRef,
                                         EntityValue, EntityRef>;

template<PQLSynonymType SYN_TYPE, StmtType StatementType,
    PatternQueryInvoker invoker>
class AbstractPatternClause : public PatternClause {
 protected:
  AbstractPatternClause(const PQLQuerySynonymProxy &synonym,
                        ClauseArgumentPtr leftArg) :
      PatternClause(synonym, std::move(leftArg), SYN_TYPE) {
  }

 public:
  PQLQueryResult *evaluateOn(const QueryExecutorAgent &agent) const override {
    StmtRef leftStatement = {StatementType, 0};
    EntityRef leftVar = leftArg->toEntityRef();
    const PQLSynonymName &synName = synonym->getName();
    leftStatement = agent.transformArg(synName, leftStatement);
    leftVar = agent.transformArg(leftArg->getName(), leftVar);

    if (!agent.isValid(leftStatement) || !agent.isValid(leftVar)) {
      return new PQLQueryResult();
    }

    QueryResultPtr<StmtValue, EntityValue> result =
        invoker(agent, leftStatement, leftVar);
    PQLQueryResultBuilder<StmtValue, EntityValue> builder;
    builder.setLeftName(synName);
    builder.setRightName(leftArg.get());
    builder.setLeftRef(leftStatement);
    builder.setRightRef(leftVar);
    return builder.build(result.get());
  }

  ComplexityScore getComplexityScore(const OverrideTable *table)
  const override {
    if (table->contains(leftArg->getName())) {
      return COMPLEXITY_QUERY_CONSTANT;
    }
    return COMPLEXITY_QUERY_SYN_CONTAINER;
  }
};
