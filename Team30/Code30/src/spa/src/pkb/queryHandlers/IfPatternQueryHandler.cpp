#include "IfPatternQueryHandler.h"
#include "ArgValidators.h"

IfPatternQueryHandler::IfPatternQueryHandler(
    PkbStmtEntQueryInvoker *invoker,
    IfPatternStorage *storage)
    : PkbStmtEntQueryHandler(invoker, storage) {
  PkbStmtEntQueryHandler::setLeftValidator(ifLeftArgValidator);
  PkbStmtEntQueryHandler::setRightValidator(varRightArgValidator);
  PkbStmtEntQueryHandler::setIsPattern(true);
}
