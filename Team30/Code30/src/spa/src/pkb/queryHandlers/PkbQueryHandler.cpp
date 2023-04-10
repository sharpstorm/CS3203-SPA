#include "PkbQueryHandler.h"

#include <memory>

#include "AssignsQueryHandler.h"
#include "CFGsQueryHandler.h"
#include "CallsQueryHandler.h"
#include "FollowsQueryHandler.h"
#include "IfPatternQueryHandler.h"
#include "ModifiesQueryHandler.h"
#include "ParentQueryHandler.h"
#include "UsesQueryHandler.h"
#include "WhilePatternQueryHandler.h"

using std::make_unique;

PkbQueryHandler::PkbQueryHandler(PKB *pkb)
    : stmtStmtQueryInvoker(make_unique<PkbStmtStmtQueryInvoker>(
          pkb->structureProvider, pkb->stmtPredicateFactory)),
      stmtEntQueryInvoker(make_unique<PkbStmtEntQueryInvoker>(
          pkb->structureProvider, pkb->entityMappingProvider,
          pkb->stmtPredicateFactory, pkb->entityPredicateFactory)),
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

EntityValueSet PkbQueryHandler::getSymbolsOfType(EntityType entityType) const {
  return designEntityHandler->getSymbolsOfType(entityType);
}

EntityValue PkbQueryHandler::getVariableByIndex(EntityIdx index) const {
  return designEntityHandler->getVariableByIndex(index);
}

EntityValue PkbQueryHandler::getConstantByIndex(EntityIdx index) const {
  return designEntityHandler->getConstantByIndex(index);
}

EntityIdx PkbQueryHandler::getIndexOfVariable(EntityValue name) const {
  return designEntityHandler->getIndexOfVariable(name);
}

EntityIdx PkbQueryHandler::getIndexOfConstant(EntityValue name) const {
  return designEntityHandler->getIndexOfConstant(name);
}

StmtValueSet PkbQueryHandler::getStatementsOfType(StmtType stmtType) const {
  return designEntityHandler->getStatementsOfType(stmtType);
}

StmtType PkbQueryHandler::getStatementType(StmtValue stmtNo) const {
  return designEntityHandler->getStatementType(stmtNo);
}

QueryResultPtr<StmtValue, EntityValue> PkbQueryHandler::queryUses(
    StmtRef leftArg, EntityRef rightArg) const {
  return usesHandler->query(&leftArg, &rightArg);
}

QueryResultPtr<EntityValue, EntityValue> PkbQueryHandler::queryUses(
    EntityRef leftArg, EntityRef rightArg) const {
  return usesHandler->query(&leftArg, &rightArg);
}
QueryResultPtr<StmtValue, EntityValue> PkbQueryHandler::queryModifies(
    StmtRef leftArg, EntityRef rightArg) const {
  return modifiesHandler->query(&leftArg, &rightArg);
}

QueryResultPtr<EntityValue, EntityValue> PkbQueryHandler::queryModifies(
    EntityRef leftArg, EntityRef rightArg) const {
  return modifiesHandler->query(&leftArg, &rightArg);
}

QueryResultPtr<StmtValue, PatternTrie *> PkbQueryHandler::queryAssigns(
    StmtRef leftArg) const {
  return assignHandler->queryAssigns(leftArg);
}

QueryResultPtr<EntityValue, EntityValue> PkbQueryHandler::queryCalls(
    EntityRef leftArg, EntityRef rightArg) const {
  return callsHandler->query(&leftArg, &rightArg);
}

QueryResultPtr<EntityValue, EntityValue> PkbQueryHandler::queryCallsStar(
    EntityRef leftArg, EntityRef rightArg) const {
  return callsTHandler->query(&leftArg, &rightArg);
}

QueryResultPtr<StmtValue, EntityValue> PkbQueryHandler::queryIfPattern(
    StmtRef leftArg, EntityRef rightArg) const {
  return ifPatternHandler->query(&leftArg, &rightArg);
}

QueryResultPtr<StmtValue, EntityValue> PkbQueryHandler::queryWhilePattern(
    StmtRef leftArg, EntityRef rightArg) const {
  return whilePatternHandler->query(&leftArg, &rightArg);
}

vector<CFG *> PkbQueryHandler::queryCFGs(StmtRef leftArg) const {
  return cfgsHandler->queryCFGs(leftArg);
}

bool PkbQueryHandler::isStatementOfType(StmtType stmtType,
                                        StmtValue stmtNo) const {
  return designEntityHandler->isStatementOfType(stmtType, stmtNo);
}

bool PkbQueryHandler::isSymbolOfType(EntityType entityType,
                                     EntityValue name) const {
  return designEntityHandler->isSymbolOfType(entityType, name);
}

EntityValue PkbQueryHandler::getCalledDeclaration(StmtValue callStmt) const {
  return designEntityHandler->getCalledDeclaration(callStmt);
}

EntityValue PkbQueryHandler::getReadDeclarations(StmtValue readStmt) const {
  return modifiesHandler->getReadDeclarations(readStmt);
}

EntityValue PkbQueryHandler::getPrintDeclarations(StmtValue printStmt) const {
  return usesHandler->getPrintDeclarations(printStmt);
}
