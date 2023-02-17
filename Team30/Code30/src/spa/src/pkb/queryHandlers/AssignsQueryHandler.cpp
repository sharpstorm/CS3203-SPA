#include "AssignsQueryHandler.h"

#include <string>

using std::string;

AssignsQueryHandler::AssignsQueryHandler(const AssignStorage *assignStore)
    : assignStore(assignStore) {}

QueryResult<int, shared_ptr<IASTNode>> AssignsQueryHandler::queryAssigns(
    StmtRef stmt) const {
  QueryResult<int, shared_ptr<IASTNode>> result;
  result.add(stmt.lineNum, assignStore->get(stmt.lineNum));
  return result;
}
