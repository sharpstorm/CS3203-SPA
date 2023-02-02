#include "FollowsQueryHandler.h"

FollowsQueryHandler::FollowsQueryHandler(FollowsStorage& store) : followsStore(store) {}

QueryResult<int, int> FollowsQueryHandler::queryFollows(StmtRef s1, StmtRef s2) {
	return QueryResult<int, int>();
}