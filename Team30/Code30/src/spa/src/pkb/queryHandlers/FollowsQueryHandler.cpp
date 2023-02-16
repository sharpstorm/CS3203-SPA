
#include "FollowsQueryHandler.h"

FollowsQueryHandler::FollowsQueryHandler(
    const FollowsStorage *store, const PredicateFactory *predicateFactory,
    const IStructureMappingProvider *stuctureProvider)
    : store(store),
      predicateFactory(predicateFactory),
      structureProvider(stuctureProvider) {}

QueryResult<int, int> FollowsQueryHandler::queryFollows(StmtRef s1,
                                                        StmtRef s2) const {
  if (s1.isKnown()) {
    // (stmtNum,stmtType)
    // or (stmtNum, stmtNum)
    return store->query(s1.lineNum, predicateFactory->getPredicate(s2));
  } else if (s2.isKnown()) {
    // (stmtType, stmtNum)
    return store->query(predicateFactory->getPredicate(s1), s2.lineNum);
  } else {
    // (stmtType, stmtType)
    return store->query(structureProvider->getStatementsOfType(s1.type),
                        predicateFactory->getPredicate(s2));
  }
}

QueryResult<int, int> FollowsQueryHandler::queryFollowsStar(StmtRef s1,
                                                            StmtRef s2) const {
  if (s1.isKnown()) {
    // (stmtNum,stmtType)
    // or (stmtNum, stmtNum)
    return store->queryT(s1.lineNum, predicateFactory->getPredicate(s2));
  } else if (s2.isKnown()) {
    // (stmtType, stmtNum)
    return store->queryT(predicateFactory->getPredicate(s1), s2.lineNum);
  } else {
    // (stmtType, stmtType)
    return store->queryT(structureProvider->getStatementsOfType(s1.type),
                         predicateFactory->getPredicate(s2));
  }
}
