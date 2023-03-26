#include "ParentQueryHandler.h"
#include "BaseQueryHandler.h"
#include "ArgValidators.h"

ParentQueryHandler::ParentQueryHandler(
    PkbStmtStmtQueryInvoker *invoker,
    ParentStorage *storage) :
    PkbStmtStmtQueryHandler(invoker, storage) {
  PkbStmtStmtQueryHandler::setLeftValidator(parentLeftArgValidator);
}
