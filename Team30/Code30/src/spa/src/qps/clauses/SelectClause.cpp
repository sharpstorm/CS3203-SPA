#include <unordered_set>
#include <utility>
#include <vector>
#include <string>

#include "SelectClause.h"
#include "arguments/ClauseArgument.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"

using std::pair, std::unordered_set, std::vector, std::string;

SelectClause::SelectClause(const PQLQuerySynonym &target):
    target(target) {}

PQLQueryResult* SelectClause::evaluateOn(PkbQueryHandler* pkbQueryHandler) {
  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(target);

  if (target.isStatementType()) {
    StmtRef stmtVar = clauseArg->toStmtRef();
    unordered_set<int> pkbResult = pkbQueryHandler
        ->getStatementsOfType(stmtVar.type);
    return Clause::toQueryResult(target.getName(), pkbResult);
  }

  EntityRef entityVar = clauseArg->toEntityRef();
  unordered_set<string> pkbResult = pkbQueryHandler
      ->getSymbolsOfType(entityVar.type);
  return Clause::toQueryResult(target.getName(), pkbResult);
}

bool SelectClause::validateArgTypes(VariableTable *variables) {
  return !target.isType(PQL_SYN_TYPE_PROCEDURE);
}

SynonymList SelectClause::getUsedSynonyms() {
  return SynonymList{target.getName()};
}
