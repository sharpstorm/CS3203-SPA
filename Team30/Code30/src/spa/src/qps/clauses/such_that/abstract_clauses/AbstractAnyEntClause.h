#pragma once

#include <utility>

#include "qps/errors/QPSParserSemanticError.h"
#include "qps/clauses/InvokerTypes.h"
#include "AbstractTwoArgClause.h"
#include "RefEvalulator.h"
#include "TwoArgEvaluator.h"

template<
    EntEntInvoker entInvoker,
    StmtEntInvoker stmtInvoker,
    SynonymPredicate leftValidator,
    SynonymPredicate rightValidator>
class AbstractAnyEntClause : public AbstractTwoArgClause {
 public:
  AbstractAnyEntClause(ClauseArgumentPtr left, ClauseArgumentPtr right)
      : AbstractTwoArgClause(std::move(left), std::move(right)) {
  }

  bool validateArgTypes() const override {
    if (left->isWildcard()) {
      return false;
    }

    return AbstractTwoArgClause::validateArgTypes<
        leftValidator, rightValidator>();
  }

  PQLQueryResult *evaluateOn(const QueryExecutorAgent &agent) const override {
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

    EntityRef rightRef = RefEvalulator::makeRef(agent,
                                                right.get(),
                                                Clause::toEntityRef);

    if (isLeftStatement) {
      StmtRef leftRef = RefEvalulator::makeRef(agent,
                                               left.get(),
                                               Clause::toStmtRef);
      return TwoArgEvaluator<StmtRef, EntityRef>(agent, left.get(), right.get())
          .evaluate(leftRef, rightRef, stmtInvoker);
    } else {
      EntityRef leftRef = RefEvalulator::makeRef(agent,
                                                 left.get(),
                                                 Clause::toEntityRef);
      return TwoArgEvaluator<EntityRef, EntityRef>(agent, left.get(),
                                                   right.get())
          .evaluate(leftRef, rightRef, entInvoker);
    }
  }
};
