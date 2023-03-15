
#include "ParentQueryHandler.h"

ParentQueryHandler::ParentQueryHandler(
    const ParentStorage *store, const PredicateFactory *predicateFactory,
    const IStructureMappingProvider *structureProvider)
    : store(store),
      predicateFactory(predicateFactory),
      structureProvider(structureProvider) {}

QueryResult<int, int> ParentQueryHandler::queryParent(StmtRef s1,
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
    return store->query(structureProvider->getValuesOfType(s1.type),
                        predicateFactory->getPredicate(s2));
  }
}

QueryResult<int, int> ParentQueryHandler::queryParentStar(StmtRef s1,
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
    return store->queryT(structureProvider->getValuesOfType(s1.type),
                         predicateFactory->getPredicate(s2));
  }
}
