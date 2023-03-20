#include "ParentQueryHandler.h"
#include "BaseQueryHandler.h"

ParentQueryHandler::ParentQueryHandler(
    PkbStmtStmtQueryInvoker *invoker,
    ParentStorage *storage) :
    PkbStmtStmtQueryHandler(invoker, storage) {}
