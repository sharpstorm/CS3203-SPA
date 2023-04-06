#pragma once

#include <utility>

#include "qps/common/PQLQuerySynonym.h"
#include "AbstractPatternClause.h"

constexpr PatternQueryInvoker ifPatternInvoker =
    [](const QueryExecutorAgent &agent,
       const StmtRef &stmtRef,
       const EntityRef &entityRef)
        -> QueryResultPtr<StmtValue, EntityValue> {
      return agent->queryIfPattern(stmtRef, entityRef);
    };

class IfPatternClause : public AbstractPatternClause<
    PQL_SYN_TYPE_IF, StmtType::If, ifPatternInvoker> {
 public:
  IfPatternClause(const PQLQuerySynonymProxy &ifSynonym,
                  ClauseArgumentPtr leftArg) :
      AbstractPatternClause(ifSynonym, std::move(leftArg)) {}
};
