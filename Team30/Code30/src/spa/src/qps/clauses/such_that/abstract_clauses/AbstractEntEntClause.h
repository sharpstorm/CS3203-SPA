#pragma once

#include <utility>

#include "AbstractTwoArgClause.h"
#include "qps/clauses/InvokerTypes.h"

template <
    EntEntInvoker invoker,
    EntInvoker symmetricInvoker,
    SynonymPredicate leftValidator,
    SynonymPredicate rightValidator>
class AbstractEntEntClause: public AbstractTwoArgClause {
 public:
  AbstractEntEntClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractTwoArgClause(std::move(left), std::move(right)) {
  }

  bool validateArgTypes(VariableTable *table) override {
    return AbstractTwoArgClause::validateArgTypes<
        leftValidator, rightValidator>(table);
  }

  PQLQueryResult* evaluateOn(const QueryExecutorAgent &agent) override {
    return AbstractTwoArgClause::evaluateOn<EntityValue, EntityRef,
                                            EntityValue, EntityRef,
                                            Clause::toEntityRef,
                                            Clause::toEntityRef,
                                            invoker,
                                            symmetricInvoker>(agent);
  }
};
