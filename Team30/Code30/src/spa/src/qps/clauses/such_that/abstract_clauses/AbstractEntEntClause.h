#pragma once

#include <utility>

#include "common/Types.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/AbstractTwoArgClause.h"
#include "AbstractSuchThatClause.h"
#include "InvokerTypes.h"

template <
    EntEntInvoker invoker,
    SynonymPredicate leftValidator,
    SynonymPredicate rightValidator>
class AbstractEntEntClause: public AbstractSuchThatClause {
 public:
  AbstractEntEntClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractSuchThatClause(std::move(left), std::move(right)) {
  }
  bool validateArgTypes(VariableTable *table) override {
    return AbstractSuchThatClause::validateArgTypes<
        leftValidator, rightValidator>(table);
  }

  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) override {
    return AbstractSuchThatClause::evaluateOn<EntityValue, EntityRef,
                                              EntityValue, EntityRef,
                                              Clause::toEntityRef,
                                              Clause::toEntityRef,
                                              invoker>(pkbQueryHandler);
  }
};
