#include "PkbQueryHandler.h"

PkbQueryHandler::PkbQueryHandler(PKB* pkb)
    : followsHandler(pkb->followsStore), parentHandler(pkb->parentStore) {}

QueryResult<int, int> PkbQueryHandler::queryFollows(StmtRef s1, StmtRef s2) {
  return followsHandler.queryFollows(s1, s2);
}

QueryResult<int, int> PkbQueryHandler::queryParent(StmtRef s1, StmtRef s2) {
  return parentHandler.queryParent(s1, s2);
}
