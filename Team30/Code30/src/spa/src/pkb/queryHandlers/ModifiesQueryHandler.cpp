#include "ModifiesQueryHandler.h"
#include "ArgValidators.h"
#include "ArgTransformers.h"

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

QueryResult<StmtValue, EntityValue> ModifiesQueryHandler::queryModifies(
    StmtRef leftArg, EntityRef rightArg) const {
  return PkbStmtEntQueryHandler::query(&leftArg, &rightArg);
}

QueryResult<EntityValue, EntityValue> ModifiesQueryHandler::queryModifies(
    EntityRef leftArg, EntityRef rightArg) const {
  return PkbEntEntQueryHandler::query(&leftArg, &rightArg);
}

EntityValue ModifiesQueryHandler::getReadDeclarations(StmtValue readStmt) const {
  // assumes input is read stmt
  auto values = modifiesStorage->getByFirstArg(readStmt);
  if (values.empty()) {
    return "";
  } else {
    return *values.begin();
  }
}
