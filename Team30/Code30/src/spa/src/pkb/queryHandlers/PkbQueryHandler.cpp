#include "PkbQueryHandler.h"

PkbQueryHandler::PkbQueryHandler(PKB* pkb)
    : followsHandler(pkb->followsStore, pkb->predicateFactory,
                     pkb->structureProvider),
      parentHandler(pkb->parentStore, pkb->predicateFactory,
                    pkb->structureProvider) {}

QueryResult<int, int> PkbQueryHandler::queryFollows(StmtRef s1,
                                                    StmtRef s2) const {
  return followsHandler.queryFollows(s1, s2);
}

QueryResult<int, int> PkbQueryHandler::queryFollowsStar(StmtRef s1,
                                                        StmtRef s2) const {
  return followsHandler.queryFollowsStar(s1, s2);
}

QueryResult<int, int> PkbQueryHandler::queryParent(StmtRef s1,
                                                   StmtRef s2) const {
  return parentHandler.queryParent(s1, s2);
}

QueryResult<int, int> PkbQueryHandler::queryParentStar(StmtRef s1,
                                                       StmtRef s2) const {
  return parentHandler.queryParentStar(s1, s2);
}
