#include <string>
#include "ModifiesQueryHandler.h"

using std::string;

ModifiesQueryHandler::ModifiesQueryHandler() {}

QueryResult<int, string> ModifiesQueryHandler::queryModifies(
    StmtRef arg1, EntityRef arg2) const {
  return QueryResult<int, string>();
}

QueryResult<string, string> ModifiesQueryHandler::queryModifies(
    EntityRef arg1, EntityRef arg2) const {
  return QueryResult<string, string>();
}
