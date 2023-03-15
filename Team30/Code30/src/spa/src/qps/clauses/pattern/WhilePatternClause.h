#pragma once

#include "qps/common/PQLQuerySynonym.h"
#include "qps/common/PQLQueryResult.h"
#include "qps/clauses/PatternClause.h"
#include "AbstractPatternClause.h"

constexpr PatternQueryInvoker whilePatternInvoker =
    [](PkbQueryHandler* pkbHandler,
       const StmtRef &stmtRef,
       const EntityRef &entityRef)
        -> QueryResult<int, string> {
      return pkbHandler->queryWhilePattern(stmtRef, entityRef);
    };

class WhilePatternClause: public AbstractPatternClause<
    PQL_SYN_TYPE_WHILE, StmtType::While, whilePatternInvoker> {
 public:
  WhilePatternClause(const PQLQuerySynonym &whileSynonym,
                     ClauseArgumentPtr leftArg):
      AbstractPatternClause(whileSynonym, std::move(leftArg)) {}
};
