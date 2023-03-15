#pragma once

#include <utility>

#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/clauses/PatternClause.h"
#include "AbstractPatternClause.h"

constexpr PatternQueryInvoker ifPatternInvoker =
    [](PkbQueryHandler* pkbHandler,
       const StmtRef &stmtRef,
       const EntityRef &entityRef)
        -> QueryResult<int, string> {
      return pkbHandler->queryIfPattern(stmtRef, entityRef);
    };

class IfPatternClause: public AbstractPatternClause<
    PQL_SYN_TYPE_IF, StmtType::If, ifPatternInvoker> {
 public:
  IfPatternClause(const PQLQuerySynonym &ifSynonym,
                  ClauseArgumentPtr leftArg):
      AbstractPatternClause(ifSynonym, std::move(leftArg)) {}
};
