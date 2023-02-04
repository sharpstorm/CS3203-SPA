#pragma once

#include "../../common/Types.h"
#include "../predicates/PredicateFactory.h"
#include "../storage/FollowsStorage.h"

class FollowsQueryHandler {
 public:
  explicit FollowsQueryHandler(const FollowsStorage* followsStore,
                               const PredicateFactory* predicateFactory);

  QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2) const;

 private:
  const FollowsStorage* followsStore;
  const PredicateFactory* predicateFactory;
};
