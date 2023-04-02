#include "FollowsQueryHandler.h"

FollowsQueryHandler::FollowsQueryHandler(PkbStmtStmtQueryInvoker *invoker,
                                         FollowsTableManager *storage)
    : PkbStmtStmtQueryHandler(invoker, storage) {}
