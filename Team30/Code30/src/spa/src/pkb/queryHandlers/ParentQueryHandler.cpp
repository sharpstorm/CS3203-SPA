#include "ParentQueryHandler.h"
#include "BaseQueryHandler.h"
#include "ArgValidators.h"

ParentQueryHandler::ParentQueryHandler(
    PkbStmtStmtQueryInvoker *invoker,
    IStorage<StmtValue, StmtValue> *storage) :
    PkbStmtStmtQueryHandler(invoker, storage) {
  PkbStmtStmtQueryHandler::setLeftValidator(parentLeftArgValidator);
}
