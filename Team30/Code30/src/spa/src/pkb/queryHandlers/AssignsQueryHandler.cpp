#include "AssignsQueryHandler.h"

#include <string>

using std::string;

AssignsQueryHandler::AssignsQueryHandler() {}

QueryResult<int, shared_ptr<IASTNode>> AssignsQueryHandler::queryAssigns()
    const {
  return QueryResult<int, shared_ptr<IASTNode>>();
}

QueryResult<int, shared_ptr<IASTNode>> AssignsQueryHandler::queryAssigns(
    StmtRef) const {
  return QueryResult<int, shared_ptr<IASTNode>>();
}
