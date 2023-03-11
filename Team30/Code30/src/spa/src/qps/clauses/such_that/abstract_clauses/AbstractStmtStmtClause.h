#pragma once

#include <utility>

#include "InvokerTypes.h"
#include "AbstractTwoArgClause.h"

template <
    StmtStmtInvoker invoker,
    SynonymPredicate leftValidator,
    SynonymPredicate rightValidator>
class AbstractStmtStmtClause: public AbstractTwoArgClause {
 public:
  AbstractStmtStmtClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractTwoArgClause(std::move(left), std::move(right)) {
  }

  bool validateArgTypes(VariableTable *table) override {
    return AbstractTwoArgClause::validateArgTypes<
        leftValidator, rightValidator>(table);
  }

  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) override {
    return AbstractTwoArgClause::evaluateOn<StmtValue, StmtRef,
                                            StmtValue, StmtRef,
                                            Clause::toStmtRef,
                                            Clause::toStmtRef,
                                            invoker>(pkbQueryHandler);
  }
};
