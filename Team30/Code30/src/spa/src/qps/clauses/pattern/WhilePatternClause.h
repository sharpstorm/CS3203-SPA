#pragma once

#include <utility>

#include "qps/common/PQLQuerySynonym.h"
#include "AbstractPatternClause.h"

constexpr PatternQueryInvoker whilePatternInvoker =
    [](const QueryExecutorAgent &agent,
       const StmtRef &stmtRef,
       const EntityRef &entityRef)
        -> QueryResultPtr<StmtValue, EntityValue> {
      return agent->queryWhilePattern(stmtRef, entityRef);
    };

class WhilePatternClause : public AbstractPatternClause<
    PQL_SYN_TYPE_WHILE, StmtType::While, whilePatternInvoker> {
 public:
  WhilePatternClause(const PQLQuerySynonymProxy &whileSynonym,
                     ClauseArgumentPtr leftArg) :
      AbstractPatternClause(whileSynonym, std::move(leftArg)) {}
};
