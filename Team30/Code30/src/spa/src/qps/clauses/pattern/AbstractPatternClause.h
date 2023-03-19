#pragma once

#include <string>
#include <utility>

#include "qps/common/PQLTypes.h"
#include "common/Types.h"
#include "qps/clauses/PatternClause.h"
#include "qps/clauses/InvokerTypes.h"

using PatternQueryInvoker = QueryInvoker<StmtValue, StmtRef,
                                         EntityValue, EntityRef>;

template <PQLSynonymType SYN_TYPE, StmtType StatementType,
    PatternQueryInvoker invoker>
class AbstractPatternClause: public PatternClause {
 protected:
  AbstractPatternClause(const PQLQuerySynonym &synonym,
                        ClauseArgumentPtr leftArg):
      PatternClause(synonym, std::move(leftArg), SYN_TYPE) {
  }

  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler,
                             OverrideTable* table) {
    StmtRef leftStatement = {StatementType, 0};
    EntityRef leftVar = leftArg->toEntityRef();
    if (leftArg->canSubstitute(table)) {
      OverrideTransformer overrideTrans = table->at(leftArg->getName());
      leftVar = overrideTrans.transformArg(leftVar);
      if (!Clause::isValidRef(leftVar, pkbQueryHandler)) {
        return new PQLQueryResult();
      }
    }

    QueryResult<StmtValue, EntityValue> result =
        invoker(pkbQueryHandler, leftStatement, leftVar);
    return Clause::toQueryResult(synonym.getName(), leftArg.get(), result);
  }
};
