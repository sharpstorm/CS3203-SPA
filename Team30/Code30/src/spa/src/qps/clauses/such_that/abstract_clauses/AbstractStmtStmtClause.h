#pragma once

#include <utility>

#include "qps/clauses/InvokerTypes.h"
#include "AbstractTwoArgClause.h"

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

  bool validateArgTypes(const VariableTable *table) const override {
    return AbstractTwoArgClause::validateArgTypes<
        leftValidator, rightValidator>(table);
  }

  PQLQueryResult *evaluateOn(const QueryExecutorAgent &agent) const override {
    return AbstractTwoArgClause::abstractEvaluateOn(agent,
                                                    Clause::toStmtRef,
                                                    Clause::toStmtRef,
                                                    invoker,
                                                    symmetricInvoker);
  }
};
