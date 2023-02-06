#pragma once

#include <string>
#include <unordered_set>

#include "../../common/Types.h"
#include "../queryHandlers/DesignEntitiesQueryHandler.h"
#include "../queryHandlers/FollowsQueryHandler.h"
#include "../queryHandlers/ParentQueryHandler.h"
#include "../storage/PKB.h"

class PkbQueryHandler {
 public:
  explicit PkbQueryHandler(PKB* pkb);

  QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2) const;
  QueryResult<int, int> queryFollowsStar(StmtRef s1, StmtRef s2) const;
  QueryResult<int, int> queryParent(StmtRef s1, StmtRef s2) const;
  QueryResult<int, int> queryParentStar(StmtRef s1, StmtRef s2) const;
  std::unordered_set<std::string> getSymbolsOfType(EntityType) const;
  std::unordered_set<int> getStatementsOfType(StmtType) const;

 private:
  FollowsQueryHandler followsHandler;
  ParentQueryHandler parentHandler;
  DesignEntitiesQueryHandler designEntityHandler;
};
