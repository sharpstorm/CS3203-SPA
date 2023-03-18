#include <unordered_set>
#include <utility>
#include <vector>
#include <string>

#include "SelectClause.h"
#include "arguments/ClauseArgument.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"

using std::pair, std::unordered_set, std::vector, std::string, std::to_string;

SelectClause::SelectClause(const PQLQuerySynonym &target):
    target(target) {}

PQLQueryResult* SelectClause::evaluateOn(PkbQueryHandler* pkbQueryHandler,
                                         OverrideTable* table) {
  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(target);
  PQLSynonymName synName = target.getName();
  if (target.isStatementType()) {
    StmtRef stmtVar = clauseArg->toStmtRef();
    unordered_set<int> pkbResult = pkbQueryHandler
        ->getStatementsOfType(stmtVar.type);

    if (clauseArg->canSubstitute(table)) {
      OverrideTransformer trans = table->at(synName);
      StmtValue stmtVal = trans.getStmtValue();
      if (pkbResult.find(stmtVal) != pkbResult.end()) {
        return Clause::toQueryResult(target.getName(),
                                     unordered_set<int>({stmtVal}));
      } else {
        return Clause::toQueryResult(target.getName(), unordered_set<int>());
      }
    }
    return Clause::toQueryResult(target.getName(), pkbResult);
  }

  EntityRef entityVar = clauseArg->toEntityRef();
  unordered_set<string> pkbResult = pkbQueryHandler
      ->getSymbolsOfType(entityVar.type);
  if (clauseArg->canSubstitute(table)) {
    OverrideTransformer trans = table->at(synName);
    EntityValue entVal;
    if (target.getType() == PQL_SYN_TYPE_CONSTANT) {
      entVal = to_string(trans.getStmtValue());
    } else {
      entVal = trans.getEntityValue();
    }
    if (pkbResult.find(entVal) != pkbResult.end()) {
      return Clause::toQueryResult(target.getName(),
                                   unordered_set<string>({entVal}));
    } else {
      return Clause::toQueryResult(target.getName(), unordered_set<string>());
    }
  }

  return Clause::toQueryResult(target.getName(), pkbResult);
}

bool SelectClause::validateArgTypes(VariableTable *variables) {
  return !target.isType(PQL_SYN_TYPE_PROCEDURE);
}

SynonymList SelectClause::getUsedSynonyms() {
  return SynonymList{target.getName()};
}
