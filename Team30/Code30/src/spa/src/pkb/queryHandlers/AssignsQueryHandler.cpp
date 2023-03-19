#include "AssignsQueryHandler.h"

#include "common/pattern/PatternTrie.h"

AssignsQueryHandler::AssignsQueryHandler(const AssignStorage *assignStore)
    : assignStore(assignStore) {}

QueryResult<StmtValue, PatternTrie *> AssignsQueryHandler::queryAssigns(
    StmtRef stmt) const {
  QueryResult<StmtValue, PatternTrie *> result;
  if (!stmt.isKnown()) {
    return result;
  }
  result.add(stmt.lineNum, assignStore->get(stmt.lineNum).get());
  return result;
}
