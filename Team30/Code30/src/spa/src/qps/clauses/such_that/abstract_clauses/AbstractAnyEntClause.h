#pragma once

#include <utility>
#include <unordered_set>

#include "qps/errors/QPSParserSemanticError.h"
#include "qps/clauses/InvokerTypes.h"
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

  PQLQueryResult* evaluateOn(const QueryExecutorAgent &agent) override {
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
      constexpr SymmetricQueryInvoker<StmtValue, StmtRef> dummyInvoker =
          [](const QueryExecutorAgent &agent, const StmtRef &arg) ->
          unordered_set<StmtValue> {
        return unordered_set<StmtValue>{};
      };
      return AbstractTwoArgClause::evaluateOn<StmtValue, StmtRef,
                                              EntityValue, EntityRef,
                                              Clause::toStmtRef,
                                              Clause::toEntityRef,
                                              stmtInvoker,
                                              dummyInvoker>(agent);
    } else {
      constexpr SymmetricQueryInvoker<EntityValue, EntityRef> dummyInvoker =
          [](const QueryExecutorAgent &agent, const EntityRef &arg) ->
          unordered_set<EntityValue> {
            return unordered_set<EntityValue>{};
          };
      return AbstractTwoArgClause::evaluateOn<EntityValue, EntityRef,
                                              EntityValue, EntityRef,
                                              Clause::toEntityRef,
                                              Clause::toEntityRef,
                                              entInvoker,
                                              dummyInvoker>(agent);
    }
  }
};
