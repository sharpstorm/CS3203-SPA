#pragma once

#include <string>

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

  PQLQueryResult* evaluateOn(PkbQueryHandler* pkbQueryHandler) {
    StmtRef leftStatement = {StatementType, 0};
    QueryResult<StmtValue, EntityValue> result =
        invoker(pkbQueryHandler, leftStatement, leftArg->toEntityRef());
    return Clause::toQueryResult(synonym.getName(), leftArg.get(), result);
  }
};
