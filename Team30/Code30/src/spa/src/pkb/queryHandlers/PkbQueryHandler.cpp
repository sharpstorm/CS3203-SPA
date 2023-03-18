#include "PkbQueryHandler.h"

#include "AssignsQueryHandler.h"
#include "CFGsQueryHandler.h"
#include "CallsQueryHandler.h"
#include "FollowsQueryHandler.h"
#include "IfPatternQueryHandler.h"
#include "ModifiesQueryHandler.h"
#include "ParentQueryHandler.h"
#include "UsesQueryHandler.h"
#include "WhilePatternQueryHandler.h"

PkbQueryHandler::PkbQueryHandler(PKB *pkb)
    : followsHandler(new FollowsQueryHandler(
    pkb->followsStore, pkb->predicateFactory, pkb->structureProvider)),
      parentHandler(new ParentQueryHandler(
          pkb->parentStore, pkb->predicateFactory, pkb->structureProvider)),
      usesHandler(new UsesQueryHandler(
          pkb->usesStorage, pkb->usesPStorage, pkb->predicateFactory,
          pkb->structureProvider, pkb->entityMappingProvider)),
      modifiesHandler(new ModifiesQueryHandler(
          pkb->modifiesStorage, pkb->modifiesPStorage, pkb->predicateFactory,
          pkb->structureProvider, pkb->entityMappingProvider)),
      callsHandler(new CallsQueryHandler(pkb->callsStorage,
                                         pkb->predicateFactory,
                                         pkb->entityMappingProvider)),
      cfgsHandler(
          new CFGsQueryHandler(pkb->cfgStorage,
                               pkb->entityMappingProvider,
                               pkb->structureProvider)),
      ifPatternHandler(new IfPatternQueryHandler(pkb->ifPatternStorage,
                                                 pkb->predicateFactory,
                                                 pkb->structureProvider)),
      whilePatternHandler(new WhilePatternQueryHandler(pkb->whilePatternStorage,
                                                       pkb->predicateFactory,
                                                       pkb->structureProvider)),
      designEntityHandler(new DesignEntitiesQueryHandler(
          pkb->entityMappingProvider, pkb->structureProvider)),
      assignHandler(new AssignsQueryHandler(pkb->assignStorage)) {}

QueryResult<int, int> PkbQueryHandler::queryFollows(StmtRef s1,
                                                    StmtRef s2) const {
  return followsHandler->queryFollows(s1, s2);
}

QueryResult<int, int> PkbQueryHandler::queryFollowsStar(StmtRef s1,
                                                        StmtRef s2) const {
  return followsHandler->queryFollowsStar(s1, s2);
}

QueryResult<int, int> PkbQueryHandler::queryParent(StmtRef s1,
                                                   StmtRef s2) const {
  return parentHandler->queryParent(s1, s2);
}

QueryResult<int, int> PkbQueryHandler::queryParentStar(StmtRef s1,
                                                       StmtRef s2) const {
  return parentHandler->queryParentStar(s1, s2);
}

std::unordered_set<std::string> PkbQueryHandler::getValuesOfType(
    EntityType entityType) const {
  return designEntityHandler->getValuesOfType(entityType);
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

std::unordered_set<int> PkbQueryHandler::getValuesOfType(
    StmtType stmtType) const {
  return designEntityHandler->getValuesOfType(stmtType);
}

StmtType PkbQueryHandler::getStatementType(int stmtNo) const {
  return designEntityHandler->getStatementType(stmtNo);
}

QueryResult<int, string> PkbQueryHandler::queryUses(StmtRef arg1,
                                                    EntityRef arg2) const {
  return usesHandler->queryUses(arg1, arg2);
}

QueryResult<string, string> PkbQueryHandler::queryUses(EntityRef arg1,
                                                       EntityRef arg2) const {
  return usesHandler->queryUses(arg1, arg2);
}

QueryResult<int, string> PkbQueryHandler::queryModifies(StmtRef arg1,
                                                        EntityRef arg2) const {
  return modifiesHandler->queryModifies(arg1, arg2);
}

QueryResult<string, string> PkbQueryHandler::queryModifies(
    EntityRef arg1, EntityRef arg2) const {
  return modifiesHandler->queryModifies(arg1, arg2);
}

QueryResult<int, PatternTrie *> PkbQueryHandler::queryAssigns(
    StmtRef arg1) const {
  return assignHandler->queryAssigns(arg1);
}

QueryResult<string, string> PkbQueryHandler::queryCalls(EntityRef arg1,
                                                        EntityRef arg2) const {
  return callsHandler->queryCalls(arg1, arg2);
}

QueryResult<string, string> PkbQueryHandler::queryCallsStar(
    EntityRef arg1, EntityRef arg2) const {
  return callsHandler->queryCallsStar(arg1, arg2);
}

QueryResult<int, string> PkbQueryHandler::queryIfPattern(StmtRef arg1,
                                                         EntityRef arg2) const {
  return ifPatternHandler->queryIfPattern(arg1, arg2);
}

QueryResult<int, string> PkbQueryHandler::queryWhilePattern(
    StmtRef arg1, EntityRef arg2) const {
  return whilePatternHandler->queryWhilePattern(arg1, arg2);
}

vector<CFG*> PkbQueryHandler::queryCFGs(StmtRef arg1) const {
  return cfgsHandler->queryCFGs(arg1);
}

bool PkbQueryHandler::isStatementOfType(StmtType stmtType,
                                                   int stmtNo) const {
  return designEntityHandler->isStatementOfType(stmtType, stmtNo);
}

bool PkbQueryHandler::isSymbolOfType(EntityType entityType,
                                                string name) const {
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

