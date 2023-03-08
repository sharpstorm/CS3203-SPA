#pragma once

#include <utility>

#include "common/Types.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/such_that/AbstractSuchThatClause.h"
#include "InvokerTypes.h"

template <
    StmtStmtInvoker invoker,
    SynonymPredicate leftValidator,
    SynonymPredicate rightValidator>
class AbstractStmtStmtClause: public AbstractSuchThatClause {
 public:
  AbstractStmtStmtClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractSuchThatClause(std::move(left), std::move(right)) {
  }

  bool validateArgTypes(VariableTable *table) override {
    return AbstractSuchThatClause::validateArgTypes<
        leftValidator, rightValidator>(table);
  }

  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) override {
    return AbstractSuchThatClause::evaluateOn<StmtValue, StmtRef,
                                              StmtValue, StmtRef,
                                              Clause::toStmtRef,
                                              Clause::toStmtRef,
                                              invoker>(pkbQueryHandler);
  }
};
