#pragma once

#include <utility>

#include "common/Types.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/AbstractTwoArgClause.h"
#include "AbstractSuchThatClause.h"

typedef QueryInvoker<EntityValue, EntityRef,
                     EntityValue, EntityRef> EntEntInvoker;

template <
    EntEntInvoker invoker,
    SynonymPredicate leftValidator,
    SynonymPredicate rightValidator>
class AbstractEntEntClause: public AbstractSuchThatClause<
    EntityValue, EntityRef, EntityValue, EntityRef> {
 public:
  AbstractEntEntClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractSuchThatClause(std::move(left), std::move(right)) {
  }
  bool validateArgTypes(VariableTable *table) {
    return AbstractSuchThatClause::validateArgTypes<
        leftValidator, rightValidator>(table);
  }

  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) {
    return AbstractSuchThatClause::evaluateOn<Clause::toEntityRef,
                                              Clause::toEntityRef,
                                              invoker>(pkbQueryHandler);
  }
};
