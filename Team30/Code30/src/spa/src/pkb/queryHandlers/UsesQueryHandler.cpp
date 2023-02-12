#include <string>
#include "UsesQueryHandler.h"

using std::string;

UsesQueryHandler::UsesQueryHandler() {}

QueryResult<int, string> UsesQueryHandler::queryUses(StmtRef arg1,
                                                     EntityRef arg2) const {
  return QueryResult<int, string>();
}

QueryResult<string, string> UsesQueryHandler::queryUses(EntityRef arg1,
                                                        EntityRef arg2) const {
  return QueryResult<string, string>();
}
