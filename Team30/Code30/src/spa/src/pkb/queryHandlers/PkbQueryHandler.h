#pragma once

#include "../../common/Types.h"
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

 private:
  FollowsQueryHandler followsHandler;
  ParentQueryHandler parentHandler;
};
