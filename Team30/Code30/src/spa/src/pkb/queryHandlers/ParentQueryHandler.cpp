#include "ParentQueryHandler.h"
#include "BaseQueryHandler.h"

ParentQueryHandler::ParentQueryHandler(PkbStmtStmtQueryInvoker *invoker,
                                       ParentStorage *storage) :
    PkbStmtStmtQueryHandler(invoker, storage) {}

QueryResult<StmtValue, StmtValue>
ParentQueryHandler::queryParent(StmtRef leftArg, StmtRef rightArg) const {
  return PkbStmtStmtQueryHandler::query(&leftArg, &rightArg);
};
