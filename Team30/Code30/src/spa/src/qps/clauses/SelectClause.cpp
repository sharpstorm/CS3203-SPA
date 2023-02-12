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

SelectClause::SelectClause(PQLQueryVariable target):
  target(target) {}

PQLQueryResult* SelectClause::evaluateOn(
        shared_ptr<PkbQueryHandler> pkbQueryHandler) {
  PQLQueryResult* pqlQueryResult = new PQLQueryResult();
  if (target.isStatementType()) {
    StmtRef stmtVar = ClauseArgumentRef::toStmtRef(ClauseArgument(target));
    unordered_set<int> pkbResult = pkbQueryHandler
        ->getStatementsOfType(stmtVar.type);
    pqlQueryResult->addToStatementMap(
        target.name, StatementResultBuilder::buildStatementResult(pkbResult));
    return pqlQueryResult;
  }

  EntityRef entityVar = ClauseArgumentRef::toEntityRef(ClauseArgument(target));
  unordered_set<string> pkbResult = pkbQueryHandler
      ->getSymbolsOfType(entityVar.type);
  pqlQueryResult->addToEntityMap(
      target.name, EntityResultBuilder::buildEntityResult(pkbResult));
  return pqlQueryResult;
}

bool SelectClause::validateArgTypes(VariableTable *variables) {
  return target.type != PQL_SYN_TYPE_PROCEDURE;
}

bool SelectClause::usesSynonym(string varName) {
  return target.name == varName;
}
