#include "PkbQueryHandler.h"

#include <memory>

#include "AssignsQueryHandler.h"
#include "CFGsQueryHandler.h"
#include "CallsQueryHandler.h"
#include "IfPatternQueryHandler.h"
#include "ParentQueryHandler.h"
#include "UsesQueryHandler.h"
#include "WhilePatternQueryHandler.h"

using std::make_unique;

PkbQueryHandler::PkbQueryHandler(PKB *pkb)
    : stmtStmtQueryInvoker(make_unique<PkbStmtStmtQueryInvoker>(
          pkb->structureProvider, pkb->stmtPredicateFactory)),
      stmtEntQueryInvoker(make_unique<PkbStmtEntQueryInvoker>(
          pkb->structureProvider, pkb->stmtPredicateFactory,
          pkb->entityPredicateFactory)),
      entEntQueryInvoker(make_unique<PkbEntEntQueryInvoker>(
          pkb->entityMappingProvider, pkb->entityPredicateFactory)),
      followsHandler(new FollowsQueryHandler(stmtStmtQueryInvoker.get(),
                                             pkb->followsStorage)),
      followsTHandler(new FollowsQueryHandler(stmtStmtQueryInvoker.get(),
                                              pkb->followsTStorage)),
      parentHandler(new ParentQueryHandler(stmtStmtQueryInvoker.get(),
                                           pkb->parentStorage)),
      parentTHandler(new ParentQueryHandler(stmtStmtQueryInvoker.get(),
                                            pkb->parentTStorage)),
      modifiesHandler(new ModifiesQueryHandler(
          stmtEntQueryInvoker.get(), entEntQueryInvoker.get(),
          pkb->modifiesStorage, pkb->modifiesPStorage)),
      usesHandler(new UsesQueryHandler(stmtEntQueryInvoker.get(),
                                       entEntQueryInvoker.get(),
                                       pkb->usesStorage, pkb->usesPStorage)),
      callsHandler(
          new CallsQueryHandler(entEntQueryInvoker.get(), pkb->callsStorage)),
      callsTHandler(
          new CallsQueryHandler(entEntQueryInvoker.get(), pkb->callsTStorage)),
      ifPatternHandler(new IfPatternQueryHandler(stmtEntQueryInvoker.get(),
                                                 pkb->ifPatternStorage)),
      whilePatternHandler(new WhilePatternQueryHandler(
          stmtEntQueryInvoker.get(), pkb->whilePatternStorage)),
      assignHandler(new AssignsQueryHandler(pkb->assignStorage)),
      cfgsHandler(new CFGsQueryHandler(
          pkb->cfgStorage, pkb->entityMappingProvider, pkb->structureProvider)),
      designEntityHandler(new DesignEntitiesQueryHandler(
          pkb->entityMappingProvider, pkb->structureProvider)) {}

QueryResultPtr<int, int> PkbQueryHandler::queryFollows(StmtRef s1,
                                                       StmtRef s2) const {
  return followsHandler->query(&s1, &s2);
}

QueryResultPtr<int, int> PkbQueryHandler::queryFollowsStar(StmtRef s1,
                                                           StmtRef s2) const {
  return followsTHandler->query(&s1, &s2);
}

QueryResultPtr<int, int> PkbQueryHandler::queryParent(StmtRef s1,
                                                      StmtRef s2) const {
  return parentHandler->query(&s1, &s2);
}

QueryResultPtr<int, int> PkbQueryHandler::queryParentStar(StmtRef s1,
                                                          StmtRef s2) const {
  return parentTHandler->query(&s1, &s2);
}

std::unordered_set<std::string> PkbQueryHandler::getSymbolsOfType(
    EntityType entityType) const {
  return designEntityHandler->getSymbolsOfType(entityType);
}

string PkbQueryHandler::getVariableByIndex(int index) const {
  return designEntityHandler->getVariableByIndex(index);
}

string PkbQueryHandler::getConstantByIndex(int index) const {
  return designEntityHandler->getConstantByIndex(index);
}

unordered_set<int> PkbQueryHandler::getIndexOfVariable(string name) const {
  return designEntityHandler->getIndexOfVariable(name);
}

unordered_set<int> PkbQueryHandler::getIndexOfConstant(string name) const {
  return designEntityHandler->getIndexOfConstant(name);
}

std::unordered_set<int> PkbQueryHandler::getStatementsOfType(
    StmtType stmtType) const {
  return designEntityHandler->getStatementsOfType(stmtType);
}

StmtType PkbQueryHandler::getStatementType(int stmtNo) const {
  return designEntityHandler->getStatementType(stmtNo);
}

QueryResultPtr<int, string> PkbQueryHandler::queryUses(StmtRef arg1,
                                                       EntityRef arg2) const {
  return usesHandler->query(&arg1, &arg2);
}

QueryResultPtr<string, string> PkbQueryHandler::queryUses(
    EntityRef arg1, EntityRef arg2) const {
  return usesHandler->query(&arg1, &arg2);
}
QueryResultPtr<int, string> PkbQueryHandler::queryModifies(
    StmtRef arg1, EntityRef arg2) const {
  return modifiesHandler->query(&arg1, &arg2);
}

QueryResultPtr<string, string> PkbQueryHandler::queryModifies(
    EntityRef arg1, EntityRef arg2) const {
  return modifiesHandler->query(&arg1, &arg2);
}

QueryResultPtr<int, PatternTrie *> PkbQueryHandler::queryAssigns(
    StmtRef arg1) const {
  return assignHandler->queryAssigns(arg1);
}

QueryResultPtr<string, string> PkbQueryHandler::queryCalls(
    EntityRef arg1, EntityRef arg2) const {
  return callsHandler->query(&arg1, &arg2);
}

QueryResultPtr<string, string> PkbQueryHandler::queryCallsStar(
    EntityRef arg1, EntityRef arg2) const {
  return callsTHandler->query(&arg1, &arg2);
}

QueryResultPtr<int, string> PkbQueryHandler::queryIfPattern(
    StmtRef arg1, EntityRef arg2) const {
  return ifPatternHandler->query(&arg1, &arg2);
}

QueryResultPtr<int, string> PkbQueryHandler::queryWhilePattern(
    StmtRef arg1, EntityRef arg2) const {
  return whilePatternHandler->query(&arg1, &arg2);
}

vector<CFG *> PkbQueryHandler::queryCFGs(StmtRef arg1) const {
  return cfgsHandler->queryCFGs(arg1);
}

bool PkbQueryHandler::isStatementOfType(StmtType stmtType, int stmtNo) const {
  return designEntityHandler->isStatementOfType(stmtType, stmtNo);
}

bool PkbQueryHandler::isSymbolOfType(EntityType entityType, string name) const {
  return designEntityHandler->isSymbolOfType(entityType, name);
}

string PkbQueryHandler::getCalledDeclaration(int callStmt) const {
  return designEntityHandler->getCalledDeclaration(callStmt);
}

string PkbQueryHandler::getReadDeclarations(int readStmt) const {
  return modifiesHandler->getReadDeclarations(readStmt);
}

string PkbQueryHandler::getPrintDeclarations(int printStmt) const {
  return usesHandler->getPrintDeclarations(printStmt);
}
