#include "AssignsQueryHandler.h"

#include <string>

#include "common/pattern/PatternTrie.h"

using std::string;

AssignsQueryHandler::AssignsQueryHandler(const AssignStorage *assignStore)
    : assignStore(assignStore) {}

QueryResult<int, PatternTrie*> AssignsQueryHandler::queryAssigns(
    StmtRef stmt) const {
  QueryResult<int, PatternTrie*> result;
  if (!stmt.isKnown()) {
    return result;
  }
  result.add(stmt.lineNum, assignStore->get(stmt.lineNum).get());
  return result;
}
