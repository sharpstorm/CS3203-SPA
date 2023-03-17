#pragma once

#include <utility>

#include "qps/clauses/InvokerTypes.h"
#include "AbstractTwoArgClause.h"

template <
    StmtStmtInvoker invoker,
    StmtInvoker symmetricInvoker,
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

  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler,
                             OverrideTable* table) override {
    return AbstractTwoArgClause::evaluateOn<StmtValue, StmtRef,
                                            StmtValue, StmtRef,
                                            Clause::toStmtRef,
                                            Clause::toStmtRef,
                                            invoker,
                                            symmetricInvoker>
                                            (pkbQueryHandler, table);
  }
};
