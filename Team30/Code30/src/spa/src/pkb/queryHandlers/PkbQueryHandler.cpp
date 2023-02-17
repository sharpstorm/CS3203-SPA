#include "PkbQueryHandler.h"
#include "FollowsQueryHandler.h"
#include "ParentQueryHandler.h"
#include "UsesQueryHandler.h"
#include "ModifiesQueryHandler.h"

PkbQueryHandler::PkbQueryHandler(PKB *pkb)
    : followsHandler(new FollowsQueryHandler(pkb->followsStore,
                                             pkb->predicateFactory,
                                             pkb->structureProvider)),
      parentHandler(new ParentQueryHandler(pkb->parentStore,
                                           pkb->predicateFactory,
                                           pkb->structureProvider)),
      usesHandler(new UsesQueryHandler()),
      modifiesHandler(new ModifiesQueryHandler(pkb->modifiesStorage,
                                               pkb->predicateFactory,
                                               pkb->structureProvider,
                                               pkb->entityMappingProvider)),
      designEntityHandler(new DesignEntitiesQueryHandler(
          pkb->entityMappingProvider, pkb->structureProvider)) {}

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

std::unordered_set<std::string> PkbQueryHandler::getSymbolsOfType(
    EntityType entityType) const {
  return designEntityHandler->getSymbolsOfType(entityType);
}

std::unordered_set<int> PkbQueryHandler::getStatementsOfType(
    StmtType stmtType) const {
  return designEntityHandler->getStatementsOfType(stmtType);
}

QueryResult<int, string> PkbQueryHandler::queryUses(StmtRef arg1,
                                                    EntityRef arg2) const {
  return usesHandler->queryUses(arg1, arg2);
}

QueryResult<string, string> PkbQueryHandler::queryUses(EntityRef arg1,
                                                       EntityRef arg2) const {
  return usesHandler->queryUses(arg1, arg2);
}

QueryResult<int, string> PkbQueryHandler::queryModifies(
    StmtRef arg1, EntityRef arg2) const {
  return modifiesHandler->queryModifies(arg1, arg2);
}

QueryResult<string, string> PkbQueryHandler::queryModifies(
    EntityRef arg1, EntityRef arg2) const {
  return modifiesHandler->queryModifies(arg1, arg2);
}
