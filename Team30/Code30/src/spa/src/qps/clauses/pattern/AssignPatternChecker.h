#pragma once

#include "common/Types.h"
#include "qps/clauses/arguments/ExpressionArgument.h"
#include "common/pattern/PatternTrie.h"
#include "qps/executor/QueryExecutorAgent.h"

class AssignPatternChecker {
  typedef QueryResult<StmtValue, EntityValue> ConcreteQueryResult;

 private:
  const StmtRef *assignRef;
  const EntityRef *variableRef;
  const ExpressionArgument *expr;

 public:
  AssignPatternChecker(const ExpressionArgument *expr,
                       const StmtRef *assignRef,
                       const EntityRef *variableRef);
  void filterModifiesInto(
      const ConcreteQueryResult *modifiesResult,
      ConcreteQueryResult *assignResult,
      const QueryExecutorAgent &agent) const;

 private:
  bool checkTrie(const QueryExecutorAgent &agent,
                 const StmtValue &stmtNumber) const;
  bool isTrieMatch(PatternTrie *lineRoot) const;

  void checkBoolean(const ConcreteQueryResult *modifiesResult,
                    ConcreteQueryResult *assignResult,
                    const QueryExecutorAgent &agent) const;
  void checkStmt(const ConcreteQueryResult *modifiesResult,
                 ConcreteQueryResult *assignResult,
                 const QueryExecutorAgent &agent) const;
  void checkVariable(const ConcreteQueryResult *modifiesResult,
                     ConcreteQueryResult *assignResult,
                     const QueryExecutorAgent &agent) const;
  void checkBoth(const ConcreteQueryResult *modifiesResult,
                 ConcreteQueryResult *assignResult,
                 const QueryExecutorAgent &agent) const;
};
