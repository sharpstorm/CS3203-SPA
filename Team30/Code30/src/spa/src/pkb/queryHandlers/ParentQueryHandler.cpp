#include "ParentQueryHandler.h"

ParentQueryHandler::ParentQueryHandler(const ParentStorage* store)
    : parentStore(store) {}

QueryResult<int, int> ParentQueryHandler::queryParent(StmtRef s1,
                                                      StmtRef s2) const {
  return QueryResult<int, int>();
}
