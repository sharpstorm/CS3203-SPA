#pragma once

#include "../../common/Types.h"
#include "../storage/FollowsStorage.h"
#include "interfaces/IFollowsQueryHandler.h"

class FollowsQueryHandler: public IFollowsQueryHandler {
 public:
  explicit FollowsQueryHandler(const FollowsStorage* followsStore);
  QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2) const;

 private:
  const FollowsStorage* followsStore;
};
