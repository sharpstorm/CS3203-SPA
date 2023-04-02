#include "WhilePatternQueryHandler.h"
#include "ArgValidators.h"

WhilePatternQueryHandler::WhilePatternQueryHandler(
    PkbStmtEntQueryInvoker *invoker,
    WhilePatternStorage *storage)
    : PkbStmtEntQueryHandler(invoker, storage) {
  PkbStmtEntQueryHandler::setLeftValidator(whileLeftArgValidator);
  PkbStmtEntQueryHandler::setRightValidator(varRightArgValidator);
}


