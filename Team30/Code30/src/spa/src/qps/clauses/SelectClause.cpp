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
    unordered_set<int> result;
    StmtRef stmtRef = clauseArg->toStmtRef();
    if (clauseArg->canSubstitute(table)) {
      OverrideTransformer trans = table->at(synName);
      StmtValue stmtVal = trans.getStmtValue();
      if (pkbQueryHandler->isStatementOfType(stmtRef.type, stmtVal)) {
        result.insert(stmtVal);
      }
    } else {
      result = pkbQueryHandler
          ->getStatementsOfType(stmtRef.type);
    }

    return Clause::toQueryResult(target.getName(), result);
  }

  unordered_set<string> result;
  EntityRef entRef = clauseArg->toEntityRef();
  if (clauseArg->canSubstitute(table)) {
    OverrideTransformer trans = table->at(synName);
    EntityValue entVal;
    if (target.getType() == PQL_SYN_TYPE_CONSTANT) {
      entVal = to_string(trans.getStmtValue());
    } else {
      entVal = trans.getEntityValue();
    }

    if (pkbQueryHandler->isSymbolOfType(entRef.type, entVal)) {
      result.insert(entVal);
    }
  } else {
    result = pkbQueryHandler->getSymbolsOfType(entRef.type);
  }

  return Clause::toQueryResult(target.getName(), result);
}

bool SelectClause::validateArgTypes(VariableTable *variables) {
  return !target.isType(PQL_SYN_TYPE_PROCEDURE);
}

SynonymList SelectClause::getUsedSynonyms() {
  return SynonymList{target.getName()};
}
