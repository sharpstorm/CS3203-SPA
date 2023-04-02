#include "UsesQueryHandler.h"
#include "ArgValidators.h"
#include "ArgTransformers.h"

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
  PkbStmtEntQueryHandler::setIsPattern(true);
}

EntityValue UsesQueryHandler::getPrintDeclarations(StmtValue printStmt) const {
  // assumes input is print stmt
  auto values = usesStorage->getByFirstArg(printStmt);
  if (values.empty()) {
    return "";
  } else {
    return *values.begin();
  }
}
