#include "FollowsQueryHandler.h"

FollowsQueryHandler::FollowsQueryHandler(
    PkbStmtStmtQueryInvoker *invoker,
    RelationTableManager<StmtValue, StmtValue> *storage)
    : PkbStmtStmtQueryHandler(invoker, storage) {}
