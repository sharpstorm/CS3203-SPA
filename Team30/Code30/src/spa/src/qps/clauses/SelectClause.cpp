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

PQLQueryResult* SelectClause::evaluateOn(PkbQueryHandler* pkbQueryHandler, OverrideTable* table) {
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
//  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(target);
//
//  bool hasAttribute = table->find(target.getName()) != table->end();
//
//  PQLSynonymType synType = target.getType();
//  if (target.isStatementType()) {
//    StmtRef stmtVar = clauseArg->toStmtRef();
//    unordered_set<int> pkbResult = pkbQueryHandler
//        ->getStatementsOfType(stmtVar.type);
//
//    if (hasAttribute) {
//      OverrideTransformer trans = table->at(target.getName());
//      // stmt,read,print,call,while,if,assign.stmt#
//      if (trans.returnsInteger()) {
//        StmtValue overrideVal = trans.getStmtValue();
//        if (pkbResult.find(overrideVal) != pkbResult.end()) {
//          return Clause::toQueryResult(target.getName(),
//                                       unordered_set<int>({overrideVal}));
//        }
//        return new PQLQueryResult();
//      } else {
//        // read/print.varName, call.procName
//        EntityValue overrideVal = trans.getEntityValue();
//        unordered_set<string> foundSet = unordered_set<string>({overrideVal});
//        for (auto s : pkbResult) {
//          if (synType == PQL_SYN_TYPE_READ &&
//              pkbQueryHandler->getReadDeclarations(s) == overrideVal) {
//            return Clause::toQueryResult(target.getName(), foundSet);
//          } else if (synType == PQL_SYN_TYPE_PRINT &&
//              pkbQueryHandler->getPrintDeclarations(s) == overrideVal) {
//            return Clause::toQueryResult(target.getName(), foundSet);
//          } else if (pkbQueryHandler->getCalledDeclaration(s) == overrideVal) {
//            return Clause::toQueryResult(target.getName(), foundSet);
//          }
//        }
//
//        return new PQLQueryResult();
//      }
//    }
//
//    return Clause::toQueryResult(target.getName(), pkbResult);
//  }
//
//  // procedure.procName, variable.varName
//  EntityRef entityVar = clauseArg->toEntityRef();
//  unordered_set<string> pkbResult = pkbQueryHandler
//      ->getSymbolsOfType(entityVar.type);
//  // Special case: constant.value
//  if (hasAttribute) {
//    OverrideTransformer trans = table->at(target.getName());
//    if (synType == PQL_SYN_TYPE_CONSTANT) {
//      string overrideVal = to_string(trans.getStmtValue());
//      for (auto s : pkbResult) {
//        if (s == overrideVal) {
//          return Clause::toQueryResult(target.getName(), unordered_set<string>({s}));
//        }
//      }
//      return new PQLQueryResult();
//    }
//
//    if (pkbResult.find(trans.getEntityValue()) != pkbResult.end()) {
//      return Clause::toQueryResult(target.getName(), unordered_set<string>({trans.getEntityValue()}));
//    } else {
//      return new PQLQueryResult();
//    }
//  }
//
//  return Clause::toQueryResult(target.getName(), pkbResult);
}

bool SelectClause::validateArgTypes(VariableTable *variables) {
  return !target.isType(PQL_SYN_TYPE_PROCEDURE);
}

SynonymList SelectClause::getUsedSynonyms() {
  return SynonymList{target.getName()};
}
