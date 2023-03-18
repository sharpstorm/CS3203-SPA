#include "WithSelectClause.h"
#include "qps/clauses/arguments/ClauseArgumentFactory.h"

WithSelectClause::WithSelectClause(AttributedSynonym aSyn) : attrSyn(aSyn){ }

PQLQueryResult *WithSelectClause::evaluateOn(PkbQueryHandler *pkbQueryHandler,
                                             OverrideTable *table) {

  ClauseArgumentPtr clauseArg = ClauseArgumentFactory::create(attrSyn.getSyn());
  PQLQuerySynonym* clauseSyn = clauseArg->getSyn();
  PQLSynonymType synType = clauseSyn->getType();

  StmtRef stmtVar = clauseArg->toStmtRef();
  unordered_set<int> pkbResult = pkbQueryHandler
      ->getStatementsOfType(stmtVar.type);

  OverrideTransformer trans = table->at(attrSyn.getName());

  // read/print.varName, call.procName
  EntityValue overrideVal = trans.getEntityValue();
  unordered_set<int> foundSet = unordered_set<int>();
  for (auto s : pkbResult) {
    if (synType == PQL_SYN_TYPE_READ &&
        pkbQueryHandler->getReadDeclarations(s) == overrideVal) {
      foundSet.insert(s);
    } else if (synType == PQL_SYN_TYPE_PRINT &&
        pkbQueryHandler->getPrintDeclarations(s) == overrideVal) {
      foundSet.insert(s);
    } else if (pkbQueryHandler->getCalledDeclaration(s) == overrideVal) {
      foundSet.insert(s);
    }
  }

  return Clause::toQueryResult(attrSyn.getName(), foundSet);
}

bool WithSelectClause::validateArgTypes(VariableTable *variables) {
  return true;
}

SynonymList WithSelectClause::getUsedSynonyms() {
  return SynonymList({attrSyn.getName()});
}

//PQLQueryResult *WithSelectClause::fetchStatementResults(
//    PkbQueryHandler* pkbQueryHandler, ClauseArgument* clauseArg,
//    OverrideTable* table) {
//  PQLQuerySynonym* clauseSyn = clauseArg->getSyn();
//  PQLSynonymType synType = clauseSyn->getType();
//
//  StmtRef stmtVar = clauseArg->toStmtRef();
//  unordered_set<int> pkbResult = pkbQueryHandler
//      ->getStatementsOfType(stmtVar.type);
//
//  OverrideTransformer trans = table->at(attrSyn.getName());
//
//  // read/print.varName, call.procName
//  EntityValue overrideVal = trans.getEntityValue();
//  unordered_set<int> foundSet = unordered_set<int>();
//  for (auto s : pkbResult) {
//    if (synType == PQL_SYN_TYPE_READ &&
//        pkbQueryHandler->getReadDeclarations(s) == overrideVal) {
//      foundSet.insert(s);
//    } else if (synType == PQL_SYN_TYPE_PRINT &&
//        pkbQueryHandler->getPrintDeclarations(s) == overrideVal) {
//      foundSet.insert(s);
//    } else if (pkbQueryHandler->getCalledDeclaration(s) == overrideVal) {
//      foundSet.insert(s);
//    }
//  }
//
//  Clause::toQueryResult(attrSyn.getName(), foundSet)
//}
//
//PQLQueryResult *WithSelectClause::fetchEntityResults(PkbQueryHandler *pkbQueryHandler,
//                                                     ClauseArgument *clauseArg,
//                                                     OverrideTable *table) {
//  PQLQuerySynonym* clauseSyn = clauseArg->getSyn();
//  PQLSynonymType synType = clauseSyn->getType();
//
//  // procedure.procName, variable.varName
//  EntityRef entityVar = clauseArg->toEntityRef();
//  unordered_set<string> pkbResult = pkbQueryHandler
//      ->getSymbolsOfType(entityVar.type);
//  // Special case: constant.value
//  OverrideTransformer trans = table->at(attrSyn.getName());
//  if (synType == PQL_SYN_TYPE_CONSTANT) {
//    string overrideVal = to_string(trans.getStmtValue());
//    for (auto s : pkbResult) {
//      if (s == overrideVal) {
//        return Clause::toQueryResult(attrSyn.getName(), unordered_set<string>({s}));
//      }
//    }
//    return new PQLQueryResult();
//  }
//
//  if (pkbResult.find(trans.getEntityValue()) != pkbResult.end()) {
//    return Clause::toQueryResult(attrSyn.getName(), unordered_set<string>({trans.getEntityValue()}));
//  } else {
//    return new PQLQueryResult();
//  }
//}
