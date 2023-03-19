#include "ParentTQueryHandler.h"

ParentTQueryHandler::ParentTQueryHandler(PkbStmtStmtQueryInvoker *invoker,
                                         ParentTStorage *storage) :
    PkbStmtStmtQueryHandler(invoker, storage) {}

QueryResult<StmtValue, StmtValue>
ParentTQueryHandler::queryParentStar(StmtRef leftArg, StmtRef rightArg) const {
  return PkbStmtStmtQueryHandler::query(&leftArg, &rightArg);
};

