#pragma once

#include "../../common/Types.h"
#include "../storage/FollowsStorage.h"

class FollowsQueryHandler {
 public:
  explicit FollowsQueryHandler(const FollowsStorage* followsStore);

  QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2) const;

 private:
  const FollowsStorage* followsStore;
};
