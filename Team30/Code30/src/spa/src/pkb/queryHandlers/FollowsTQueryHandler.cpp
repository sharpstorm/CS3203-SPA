#include "FollowsTQueryHandler.h"

FollowsTQueryHandler::FollowsTQueryHandler(
    PkbStmtStmtQueryInvoker *invoker,
    FollowsTStorage *storage) :
    PkbStmtStmtQueryHandler(invoker, storage) {}

QueryResult<StmtValue, StmtValue>
FollowsTQueryHandler::queryFollowsStar(
    StmtRef leftArg,
    StmtRef rightArg) const {
  return PkbStmtStmtQueryHandler::query(&leftArg, &rightArg);
}

