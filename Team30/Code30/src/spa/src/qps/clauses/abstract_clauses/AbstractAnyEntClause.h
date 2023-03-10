#pragma once

#include <utility>

#include "qps/errors/QPSParserSemanticError.h"
#include "InvokerTypes.h"
#include "AbstractTwoArgClause.h"

template <
    EntEntInvoker entInvoker,
    StmtEntInvoker stmtInvoker,
    SynonymPredicate leftValidator,
    SynonymPredicate rightValidator>
class AbstractAnyEntClause : public AbstractTwoArgClause {
 public:
  AbstractAnyEntClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractTwoArgClause(std::move(left), std::move(right)) {
  }

  bool validateArgTypes(VariableTable *table) override {
    if (left->isWildcard()) {
      return false;
    }

    return AbstractTwoArgClause::validateArgTypes<
        leftValidator, rightValidator>(table);
  }

  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) override {
    if (left->isWildcard()) {
      throw QPSParserSemanticError(QPS_PARSER_ERR_INVALID_WILDCARD);
    }

    bool isLeftStatement;
    if (left->isNamed()) {
      isLeftStatement = left->synonymSatisfies(ClauseArgument::isStatement);
    } else {
      StmtRef stmtRef = left->toStmtRef();
      isLeftStatement = stmtRef.isKnown();
    }

    if (isLeftStatement) {
      return AbstractTwoArgClause::evaluateOn<StmtValue, StmtRef,
                                              EntityValue, EntityRef,
                                              Clause::toStmtRef,
                                              Clause::toEntityRef,
                                              stmtInvoker>(pkbQueryHandler);
    } else {
      return AbstractTwoArgClause::evaluateOn<EntityValue, EntityRef,
                                              EntityValue, EntityRef,
                                              Clause::toEntityRef,
                                              Clause::toEntityRef,
                                              entInvoker>(pkbQueryHandler);
    }
  }
};
