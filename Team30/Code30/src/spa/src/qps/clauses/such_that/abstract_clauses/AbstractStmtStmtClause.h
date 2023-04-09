#pragma once

#include <utility>

#include "qps/clauses/InvokerTypes.h"
#include "AbstractTwoArgClause.h"
#include "OneArgEvaluator.h"
#include "TwoArgEvaluator.h"
#include "common/Types.h"

/*
 * Because this is a templated class, the implementation must be fully
 * in the header file, or linker errors will occur
 */

template<
    StmtStmtInvoker invoker,
    StmtInvoker symmetricInvoker,
    SynonymPredicate leftValidator,
    SynonymPredicate rightValidator>
class AbstractStmtStmtClause : public AbstractTwoArgClause {
 public:
  AbstractStmtStmtClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractTwoArgClause(std::move(left), std::move(right)) {
  }

  bool validateArgTypes() const override {
    return AbstractTwoArgClause::validateArgTypes<
        leftValidator, rightValidator>();
  }

  PQLQueryResult *evaluateOn(const QueryExecutorAgent &agent) const override {
    StmtRef leftRef = RefEvalulator::makeRef(agent,
                                             left.get(),
                                             Clause::toStmtRef);
    StmtRef rightRef = RefEvalulator::makeRef(agent,
                                              right.get(),
                                              Clause::toStmtRef);

    if (isSameSynonym()) {
      return OneArgEvaluator<StmtRef>(agent, left.get())
          .evaluate(leftRef, symmetricInvoker);
    }

    return TwoArgEvaluator<StmtRef, StmtRef>(agent, left.get(), right.get())
        .evaluate(leftRef, rightRef, invoker);
  }
};
