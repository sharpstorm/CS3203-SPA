#pragma once

#include "../../common/Types.h"
#include "../storage/FollowsStorage.h"

class FollowsQueryHandler {
 public:
  explicit FollowsQueryHandler(FollowsStorage* followsStore);

  QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2);

 private:
  FollowsStorage* followsStore;
};
