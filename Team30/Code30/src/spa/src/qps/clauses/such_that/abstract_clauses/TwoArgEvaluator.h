#pragma once

#include "qps/executor/QueryExecutorAgent.h"
#include "qps/common/intermediate_result/PQLQueryResult.h"
#include "qps/clauses/InvokerTypes.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/common/intermediate_result/PQLQueryResultBuilder.h"
#include "RefEvalulator.h"

/*
 * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur
 */

template<typename LeftArgType, typename RightArgType>
class TwoArgEvaluator {
 private:
  const ClauseArgument *left;
  const ClauseArgument *right;
  const QueryExecutorAgent *agent;

 public:
  TwoArgEvaluator(const QueryExecutorAgent &agent,
                  const ClauseArgument *left,
                  const ClauseArgument *right) :
      agent(&agent), left(left), right(right) {}

  template<typename LeftResultType, typename RightResultType>
  PQLQueryResult *evaluate(
      const LeftArgType &leftArg,
      const RightArgType &rightArg,
      QueryInvoker<LeftResultType, LeftArgType,
                   RightResultType, RightArgType> diffSynInvoker) const {
    if (!agent->isValid(leftArg) || !agent->isValid(rightArg)) {
      return new PQLQueryResult();
    }

    auto queryResult = diffSynInvoker(*agent, leftArg, rightArg);
    PQLQueryResultBuilder<LeftResultType, RightResultType> builder;
    builder.setLeftName(left);
    builder.setRightName(right);
    builder.setLeftRef(leftArg);
    builder.setRightRef(rightArg);
    return builder.build(queryResult.get());
  }
};
