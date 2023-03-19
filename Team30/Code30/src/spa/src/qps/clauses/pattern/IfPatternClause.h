#pragma once

#include <utility>

#include "qps/common/PQLQuerySynonym.h"
#include "AbstractPatternClause.h"

constexpr PatternQueryInvoker ifPatternInvoker =
    [](PkbQueryHandler* pkbHandler,
       const StmtRef &stmtRef,
       const EntityRef &entityRef)
        -> QueryResult<StmtValue, EntityValue> {
      return pkbHandler->queryIfPattern(stmtRef, entityRef);
    };

class IfPatternClause: public AbstractPatternClause<
    PQL_SYN_TYPE_IF, StmtType::If, ifPatternInvoker> {
 public:
  IfPatternClause(const PQLQuerySynonymProxy &ifSynonym,
                  ClauseArgumentPtr leftArg):
      AbstractPatternClause(ifSynonym, std::move(leftArg)) {}
};
