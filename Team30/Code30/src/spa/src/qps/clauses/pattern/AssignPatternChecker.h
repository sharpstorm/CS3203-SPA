#pragma once

#include "common/Types.h"
#include "qps/clauses/arguments/ExpressionArgument.h"
#include "common/pattern/PatternTrie.h"
#include "qps/executor/QueryExecutorAgent.h"

class AssignPatternChecker {
 private:
  const StmtRef *assignRef;
  const EntityRef *variableRef;
  const ExpressionArgument *expr;

 public:
  AssignPatternChecker(const ExpressionArgument *expr,
                       const StmtRef *assignRef,
                       const EntityRef *variableRef);
  void filterModifiesInto(
      const QueryResult<StmtValue, EntityValue> *modifiesResult,
      QueryResult<StmtValue, EntityValue> *assignResult,
      const QueryExecutorAgent &agent) const;

 private:
  bool checkTrie(const QueryExecutorAgent &agent,
                 const StmtValue &stmtNumber) const;

  void checkBoolean(const QueryResult<StmtValue, EntityValue> *modifiesResult,
                    QueryResult<StmtValue, EntityValue> *assignResult,
                    const QueryExecutorAgent &agent) const;
  void checkStmt(const QueryResult<StmtValue, EntityValue> *modifiesResult,
                 QueryResult<StmtValue, EntityValue> *assignResult,
                 const QueryExecutorAgent &agent) const;
  void checkVariable(const QueryResult<StmtValue, EntityValue> *modifiesResult,
                     QueryResult<StmtValue, EntityValue> *assignResult,
                     const QueryExecutorAgent &agent) const;
  void checkBoth(const QueryResult<StmtValue, EntityValue> *modifiesResult,
                 QueryResult<StmtValue, EntityValue> *assignResult,
                 const QueryExecutorAgent &agent) const;
};
