#pragma once

#include <memory>
#include <utility>

#include "qps/common/PQLQuerySynonym.h"
#include "AbstractPatternClause.h"

using std::unique_ptr;

constexpr PatternQueryInvoker ifPatternInvoker =
    [](const QueryExecutorAgent &agent,
       const StmtRef &stmtRef,
       const EntityRef &entityRef)
        -> unique_ptr<QueryResult<StmtValue, EntityValue>> {
      return agent->queryIfPattern(stmtRef, entityRef);
    };

class IfPatternClause: public AbstractPatternClause<
    PQL_SYN_TYPE_IF, StmtType::If, ifPatternInvoker> {
 public:
  IfPatternClause(const PQLQuerySynonymProxy &ifSynonym,
                  ClauseArgumentPtr leftArg):
      AbstractPatternClause(ifSynonym, std::move(leftArg)) {}
};
