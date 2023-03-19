#include "CallsTQueryHandler.h"
#include "ArgValidators.h"
#include "ArgTransformers.h"

CallsTQueryHandler::CallsTQueryHandler(PkbEntEntQueryInvoker *invoker,
                                       CallsTStorage *storage)
    : PkbEntEntQueryHandler(invoker, storage) {
  PkbEntEntQueryHandler::setLeftValidator(callsArgValidator);
  PkbEntEntQueryHandler::setRightValidator(callsArgValidator);
  PkbEntEntQueryHandler::setLeftTransformer(procArgTransformer);
}

QueryResult<string, string>
CallsTQueryHandler::queryCallsStar(EntityRef leftArg,
                                   EntityRef rightArg) const {
  return PkbEntEntQueryHandler::query(&leftArg, &rightArg);
}
