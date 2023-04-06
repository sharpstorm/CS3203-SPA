#include "ModifiesQueryHandler.h"

#include "ArgTransformers.h"
#include "ArgValidators.h"

ModifiesQueryHandler::ModifiesQueryHandler(PkbStmtEntQueryInvoker *invoker,
                                           PkbEntEntQueryInvoker *pInvoker,
                                           ModifiesStorage *storage,
                                           ModifiesPStorage *pStorage)
    : PkbStmtEntQueryHandler(invoker, storage),
      PkbEntEntQueryHandler(pInvoker, pStorage),
      modifiesStorage(storage) {
  PkbStmtEntQueryHandler::setLeftValidator(modifiesLeftArgValidator);
  PkbStmtEntQueryHandler::setRightValidator(varRightArgValidator);
  PkbEntEntQueryHandler::setLeftValidator(procLeftArgValidator);
  PkbEntEntQueryHandler::setRightValidator(varRightArgValidator);
  PkbEntEntQueryHandler::setLeftTransformer(procArgTransformer);
}

EntityValue ModifiesQueryHandler::getReadDeclarations(
    StmtValue readStmt) const {
  // assumes input is read stmt
  auto values = modifiesStorage->getByFirstArg(readStmt);
  if (values.empty()) {
    return "";
  } else {
    return *values.begin();
  }
}
