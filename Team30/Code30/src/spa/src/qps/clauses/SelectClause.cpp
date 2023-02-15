#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

#include "SelectClause.h"
#include "ClauseArgument.h"
#include "qps/common/adapters/ClauseArgumentRef.h"
#include "qps/common/adapters/StatementResultBuilder.h"
#include "qps/common/adapters/EntityResultBuilder.h"

using std::pair, std::unordered_set, std::vector, std::shared_ptr;

SelectClause::SelectClause(PQLQuerySynonym target):
  target(target) {}

PQLQueryResult* SelectClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  PQLQueryResult* pqlQueryResult = new PQLQueryResult();
  ClauseArgument clauseArg = ClauseArgument(target);
  if (target.isStatementType()) {

    StmtRef stmtVar = ClauseArgumentRef::toStmtRef(&clauseArg);
    unordered_set<int> pkbResult = pkbQueryHandler
        ->getStatementsOfType(stmtVar.type);
    pqlQueryResult->addToStatementMap(
        target.getName(), StatementResultBuilder::buildStatementResult(pkbResult));
    return pqlQueryResult;
  }

  EntityRef entityVar = ClauseArgumentRef::toEntityRef(&clauseArg);
  unordered_set<string> pkbResult = pkbQueryHandler
      ->getSymbolsOfType(entityVar.type);
  pqlQueryResult->addToEntityMap(
      target.getName(), EntityResultBuilder::buildEntityResult(pkbResult));
  return pqlQueryResult;
}

bool SelectClause::validateArgTypes(VariableTable *variables) {
  return !target.isType(PQL_SYN_TYPE_PROCEDURE);
}

bool SelectClause::usesSynonym(string varName) {
  return target.getName() == varName;
}
