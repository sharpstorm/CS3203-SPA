#include "ParentTQueryHandler.h"

ParentTQueryHandler::ParentTQueryHandler(
    PkbStmtStmtQueryInvoker *invoker,
    ParentTStorage *storage) :
    PkbStmtStmtQueryHandler(invoker, storage) {}

