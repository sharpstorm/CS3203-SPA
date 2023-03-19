#include "FollowsTQueryHandler.h"

FollowsTQueryHandler::FollowsTQueryHandler(
    PkbStmtStmtQueryInvoker *invoker,
    FollowsTStorage *storage) :
    PkbStmtStmtQueryHandler(invoker, storage) {}
