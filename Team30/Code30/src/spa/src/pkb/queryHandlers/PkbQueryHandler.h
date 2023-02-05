#pragma once

#include "../../common/Types.h"
#include "../queryHandlers/FollowsQueryHandler.h"
#include "../queryHandlers/ParentQueryHandler.h"
#include "../storage/PKB.h"

class PkbQueryHandler : public IFollowsQueryHandler, IParentQueryHandler {
 public:
  explicit PkbQueryHandler(PKB* pkb);

  QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2);
  QueryResult<int, int> queryParent(StmtRef s1, StmtRef s2);

 private:
  FollowsQueryHandler followsHandler;
  ParentQueryHandler parentHandler;
};
