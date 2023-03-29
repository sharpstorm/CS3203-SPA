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
      return AbstractTwoArgClause::abstractEvaluateOn(
          agent,
          Clause::toStmtRef,
          Clause::toEntityRef,
          stmtInvoker,
          AbstractAnyEntClause::dummySymmetricInvoker<StmtValue, StmtRef>);
    } else {
      return AbstractTwoArgClause::abstractEvaluateOn(
          agent,
          Clause::toEntityRef,
          Clause::toEntityRef,
          entInvoker,
          AbstractAnyEntClause::dummySymmetricInvoker<EntityValue, EntityRef>);
    }
  }

 private:
  template <class ValueType, class RefType>
  static unordered_set<ValueType> dummySymmetricInvoker(
      const QueryExecutorAgent &agent, const RefType &arg) {
    return {};
  }
};
