#pragma once

#include <utility>

#include "AbstractTwoArgClause.h"
#include "qps/clauses/InvokerTypes.h"

template<
    EntEntInvoker invoker,
    EntInvoker symmetricInvoker,
    SynonymPredicate leftValidator,
    SynonymPredicate rightValidator>
class AbstractEntEntClause : public AbstractTwoArgClause {
 public:
  AbstractEntEntClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractTwoArgClause(std::move(left), std::move(right)) {
  }

  bool validateArgTypes() const override {
    return AbstractTwoArgClause::validateArgTypes<
        leftValidator, rightValidator>();
  }

  PQLQueryResult *evaluateOn(const QueryExecutorAgent &agent) const override {
    return AbstractTwoArgClause::abstractEvaluateOn(agent,
                                                    Clause::toEntityRef,
                                                    Clause::toEntityRef,
                                                    invoker,
                                                    symmetricInvoker);
  }
};
