#pragma once

#include <string>
#include <unordered_set>

#include "pkb/storage/PKB.h"
#include "FollowsQueryHandler.h"
#include "DesignEntitiesQueryHandler.h"
#include "ParentQueryHandler.h"
#include "pkb/queryHandlers/interfaces/IPkbQueryHandler.h"

class PkbQueryHandler : public IPkbQueryHandler {
 public:
  explicit PkbQueryHandler(PKB *pkb);

  QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2) const override;
  QueryResult<int, int> queryFollowsStar(StmtRef s1, StmtRef s2) const override;
  QueryResult<int, int> queryParent(StmtRef s1, StmtRef s2) const override;
  QueryResult<int, int> queryParentStar(StmtRef s1, StmtRef s2) const override;
  std::unordered_set<std::string> getSymbolsOfType(EntityType) const override;
  std::unordered_set<int> getStatementsOfType(StmtType) const override;

 private:
  FollowsQueryHandler followsHandler;
  ParentQueryHandler parentHandler;
  DesignEntitiesQueryHandler designEntityHandler;
};
