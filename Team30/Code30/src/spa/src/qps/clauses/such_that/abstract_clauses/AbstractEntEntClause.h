#pragma once

#include <utility>

#include "AbstractTwoArgClause.h"
#include "qps/clauses/InvokerTypes.h"
#include "RefEvalulator.h"
#include "OneArgEvaluator.h"
#include "TwoArgEvaluator.h"

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
    EntityRef leftRef = RefEvalulator::makeRef(agent,
                                             left.get(),
                                             Clause::toEntityRef);
    EntityRef rightRef = RefEvalulator::makeRef(agent,
                                              right.get(),
                                              Clause::toEntityRef);

    if (isSameSynonym()) {
      return OneArgEvaluator<EntityRef>(agent, left.get())
          .evaluate(leftRef, symmetricInvoker);
    }

    return TwoArgEvaluator<EntityRef, EntityRef>(agent, left.get(), right.get())
        .evaluate(leftRef, rightRef, invoker);
  }
};
