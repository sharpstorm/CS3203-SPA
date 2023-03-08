#pragma once

#include <utility>

#include "common/Types.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "AbstractTwoArgClause.h"
#include "InvokerTypes.h"

template <
    EntEntInvoker invoker,
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

  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) override {
    return AbstractTwoArgClause::evaluateOn<EntityValue, EntityRef,
                                            EntityValue, EntityRef,
                                            Clause::toEntityRef,
                                            Clause::toEntityRef,
                                            invoker>(pkbQueryHandler);
  }
};
