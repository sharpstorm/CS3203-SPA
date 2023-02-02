#pragma once

#include "../../common/Types.h"
#include "../storage/PKB.h"
#include "../queryHandlers/FollowsQueryHandler.h"
#include "../queryHandlers/ParentQueryHandler.h"
class PkbQueryHandler {
public:
	PkbQueryHandler(PKB& pkb);

	QueryResult<int, int> queryFollows(StmtRef s1, StmtRef s2);
	QueryResult<int, int> queryParent(StmtRef s1, StmtRef s2);

private:
	FollowsQueryHandler followsHandler;
	ParentQueryHandler parentHandler;
};