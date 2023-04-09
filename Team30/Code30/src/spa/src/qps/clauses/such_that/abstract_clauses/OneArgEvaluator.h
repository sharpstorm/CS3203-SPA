#pragma once

#include "RefEvalulator.h"
#include "qps/common/intermediate_result/PQLQueryResultBuilder.h"

template<typename ArgType>
class OneArgEvaluator {
 private:
  const ClauseArgument *arg;
  const QueryExecutorAgent *agent;

 public:
  OneArgEvaluator(const QueryExecutorAgent &agent,
                  const ClauseArgument *arg) :
      agent(&agent), arg(arg) {}

  template<typename ResultType>
  PQLQueryResult *evaluate(
      const ArgType &ref,
      SymmetricQueryInvoker<ResultType, ArgType> sameSynInvoker) const {
    if (!agent->isValid(ref)) {
      return new PQLQueryResult();
    }

    QueryResultSet<ResultType> queryResult = sameSynInvoker(*agent, ref);
    PQLQueryResultBuilder<ResultType, StmtValue> builder;
    builder.setLeftName(arg);
    builder.setLeftRef(ref);
    return builder.build(queryResult);
  }
};