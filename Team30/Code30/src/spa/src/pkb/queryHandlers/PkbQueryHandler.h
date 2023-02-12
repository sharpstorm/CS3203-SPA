#pragma once

#include <string>
#include <unordered_set>
#include <memory>

#include "pkb/storage/PKB.h"
#include "pkb/queryHandlers/interfaces/IPkbQueryHandler.h"

#include "DesignEntitiesQueryHandler.h"

using std::unordered_set;
using std::string;
using std::unique_ptr;

class PkbQueryHandler : public IPkbQueryHandler {
 public:
  explicit PkbQueryHandler(PKB *pkb);

  QueryResult<int, int> queryFollows(StmtRef, StmtRef) const override;
  QueryResult<int, int> queryFollowsStar(StmtRef, StmtRef) const override;
  QueryResult<int, int> queryParent(StmtRef, StmtRef) const override;
  QueryResult<int, int> queryParentStar(StmtRef, StmtRef) const override;
  QueryResult<int, string> queryUses(StmtRef, EntityRef) const override;
  QueryResult<string, string> queryUses(EntityRef, EntityRef) const override;
  QueryResult<int, string> queryModifies(StmtRef, EntityRef) const override;
  QueryResult<string, string> queryModifies(EntityRef,
                                            EntityRef) const override;

  unordered_set<string> getSymbolsOfType(EntityType) const override;
  unordered_set<int> getStatementsOfType(StmtType) const override;

 private:
  unique_ptr<IFollowsQueryHandler> followsHandler;
  unique_ptr<IParentQueryHandler> parentHandler;
  unique_ptr<IUsesQueryHandler> usesHandler;
  unique_ptr<IModifiesQueryHandler> modifiesHandler;
  unique_ptr<IDesignEntitiesQueryHandler> designEntityHandler;
};
