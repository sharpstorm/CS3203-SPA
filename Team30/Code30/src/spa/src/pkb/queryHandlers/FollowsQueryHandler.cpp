#include "FollowsQueryHandler.h"

FollowsQueryHandler::FollowsQueryHandler(const FollowsStorage* store)
    : followsStore(store) {}

QueryResult<int, int> FollowsQueryHandler::queryFollows(StmtRef s1,
                                                        StmtRef s2) const {
  followsStore->test();
  return QueryResult<int, int>();
}
