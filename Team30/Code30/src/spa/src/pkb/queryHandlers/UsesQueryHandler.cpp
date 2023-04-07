#include "UsesQueryHandler.h"

#include "ArgTransformers.h"
#include "ArgValidators.h"

UsesQueryHandler::UsesQueryHandler(
    PkbStmtEntQueryInvoker *invoker,
    PkbEntEntQueryInvoker *pInvoker,
    UsesStorage *storage,
    UsesPStorage *pStorage)
    : PkbStmtEntQueryHandler(invoker, storage),
    PkbEntEntQueryHandler(pInvoker, pStorage),
    usesStorage(storage) {
  PkbStmtEntQueryHandler::setLeftValidator(usesLeftArgValidator);
  PkbStmtEntQueryHandler::setRightValidator(varRightArgValidator);
  PkbEntEntQueryHandler::setLeftValidator(procLeftArgValidator);
  PkbEntEntQueryHandler::setRightValidator(varRightArgValidator);
  PkbEntEntQueryHandler::setLeftTransformer(procArgTransformer);
}

EntityValue UsesQueryHandler::getPrintDeclarations(StmtValue printStmt) const {
  // assumes input is print stmt
  auto values = usesStorage->getRightValIter(printStmt);
  EntityValue result;
  if ((result = values->getNext()).empty()) {
    return "";
  } else {
    return result;
  }
}
