#include "AssignsQueryHandler.h"

#include "common/pattern/PatternTrie.h"

AssignsQueryHandler::AssignsQueryHandler(const AssignStorage *assignStore)
    : assignStore(assignStore) {}

unique_ptr<QueryResult<StmtValue, PatternTrie *>>
AssignsQueryHandler::queryAssigns(StmtRef stmt) const {
  QueryResult<StmtValue, PatternTrie *> result;
  if (!stmt.isKnown()) {
    // todo: check whether is nullptr
    return make_unique<QueryResult<StmtValue, PatternTrie *>>();
  }
  result.add(stmt.getValue(), assignStore->get(stmt.getValue()).get());
  return make_unique<QueryResult<StmtValue, PatternTrie *>>(result);
}
