#include "IfPatternQueryHandler.h"
#include "ArgValidators.h"

IfPatternQueryHandler::IfPatternQueryHandler(PkbStmtEntQueryInvoker *invoker,
                                             IfPatternStorage *storage)
    : PkbStmtEntQueryHandler(invoker, storage) {
  PkbStmtEntQueryHandler::setLeftValidator(ifLeftArgValidator);
  PkbStmtEntQueryHandler::setRightValidator(varRightArgValidator);
}

QueryResult<StmtValue, EntityValue> IfPatternQueryHandler::queryIfPattern(
    StmtRef leftArg, EntityRef rightArg) const {
  return PkbStmtEntQueryHandler::query(&leftArg, &rightArg);
}
