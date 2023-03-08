#pragma once

#include <utility>

#include "common/Types.h"
#include "qps/clauses/arguments/ClauseArgument.h"
#include "qps/clauses/AbstractTwoArgClause.h"
#include "AbstractSuchThatClause.h"

typedef QueryInvoker<StmtValue, StmtRef,
                     StmtValue, StmtRef> StmtStmtInvoker;

template <
    StmtStmtInvoker invoker,
    SynonymPredicate leftValidator,
    SynonymPredicate rightValidator>
class AbstractStmtStmtClause: public AbstractSuchThatClause<
    StmtValue, StmtRef, StmtValue, StmtRef> {
 public:
  AbstractStmtStmtClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractSuchThatClause(std::move(left), std::move(right)) {
  }

  bool validateArgTypes(VariableTable *table) override {
    return AbstractSuchThatClause::validateArgTypes<
        leftValidator, rightValidator>(table);
  }

  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) {
    return AbstractSuchThatClause::evaluateOn<Clause::toStmtRef,
                                              Clause::toStmtRef,
                                              invoker>(pkbQueryHandler);
  }
};
