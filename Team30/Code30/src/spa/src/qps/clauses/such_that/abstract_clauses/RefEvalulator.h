#pragma once

#include "qps/clauses/InvokerTypes.h"
#include "qps/clauses/arguments/ClauseArgument.h"

class RefEvalulator {
 public:
  template<class T>
  static T makeRef(const QueryExecutorAgent &agent,
                   const ClauseArgument *clauseArg,
                   ArgumentTransformer<T> argTransformer) {
    T ref = argTransformer(clauseArg);
    ref = agent.transformArg(clauseArg->getName(), ref);
    return ref;
  }
};
