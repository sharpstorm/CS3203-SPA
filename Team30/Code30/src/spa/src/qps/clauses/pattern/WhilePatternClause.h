#pragma once

#include <utility>

#include "qps/common/PQLQuerySynonym.h"
#include "AbstractPatternClause.h"

constexpr PatternQueryInvoker whilePatternInvoker =
    [](PkbQueryHandler* pkbHandler,
       OverrideTable* table,
       const StmtRef &stmtRef,
       const EntityRef &entityRef)
        -> QueryResult<StmtValue, EntityValue> {
      return pkbHandler->queryWhilePattern(stmtRef, entityRef);
    };

class WhilePatternClause: public AbstractPatternClause<
    PQL_SYN_TYPE_WHILE, StmtType::While, whilePatternInvoker> {
 public:
  WhilePatternClause(const PQLQuerySynonym &whileSynonym,
                     ClauseArgumentPtr leftArg):
      AbstractPatternClause(whileSynonym, std::move(leftArg)) {}
};
