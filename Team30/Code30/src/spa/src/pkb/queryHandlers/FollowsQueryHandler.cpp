#include "FollowsQueryHandler.h"

FollowsQueryHandler::FollowsQueryHandler(const FollowsStorage* store,
                                         const PredicateFactory* predFactory)
    : followsStore(store), predicateFactory(predFactory) {}

QueryResult<int, int> FollowsQueryHandler::queryFollows(StmtRef s1,
                                                        StmtRef s2) const {
  followsStore->test();
  return QueryResult<int, int>();
}
