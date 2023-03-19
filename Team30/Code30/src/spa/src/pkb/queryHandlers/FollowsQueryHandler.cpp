#include "FollowsQueryHandler.h"

FollowsQueryHandler::FollowsQueryHandler(
    PkbStmtStmtQueryInvoker *invoker,
    FollowsStorage *storage) :
    PkbStmtStmtQueryHandler(invoker, storage) {}

QueryResult<StmtValue, StmtValue>
FollowsQueryHandler::queryFollows(StmtRef leftArg, StmtRef rightArg) const {
  return PkbStmtStmtQueryHandler::query(&leftArg, &rightArg);
}
