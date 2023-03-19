#include "FollowsQueryHandler.h"

FollowsQueryHandler::FollowsQueryHandler(
    PkbStmtStmtQueryInvoker *invoker,
    FollowsStorage *storage) :
    PkbStmtStmtQueryHandler(invoker, storage) {}
