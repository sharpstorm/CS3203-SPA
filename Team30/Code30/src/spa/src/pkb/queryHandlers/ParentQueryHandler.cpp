#include "ParentQueryHandler.h"

ParentQueryHandler::ParentQueryHandler(ParentStorage* store)
    : parentStore(store) {}

QueryResult<int, int> ParentQueryHandler::queryParent(StmtRef s1, StmtRef s2) {
  return QueryResult<int, int>();
}
