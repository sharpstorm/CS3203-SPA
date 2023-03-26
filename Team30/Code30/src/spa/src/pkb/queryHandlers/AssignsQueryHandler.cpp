#include "AssignsQueryHandler.h"

#include "common/pattern/PatternTrie.h"

AssignsQueryHandler::AssignsQueryHandler(const AssignStorage *assignStore)
    : assignStore(assignStore) {}

QueryResultPtr<StmtValue, PatternTrie *>
AssignsQueryHandler::queryAssigns(StmtRef stmt) const {
  QueryResult<StmtValue, PatternTrie *> result;
  if (!stmt.isKnown()) {
    return make_unique<QueryResult<StmtValue, PatternTrie *>>();
  }
  result.add(stmt.getValue(), assignStore->get(stmt.getValue()).get());
  return make_unique<QueryResult<StmtValue, PatternTrie *>>(result);
}
