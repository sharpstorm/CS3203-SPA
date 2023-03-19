#include "CallsQueryHandler.h"
#include "ArgValidators.h"
#include "ArgTransformers.h"

CallsQueryHandler::CallsQueryHandler(
    PkbEntEntQueryInvoker *invoker, CallsStorage *storage)
    : PkbEntEntQueryHandler(invoker, storage) {
  PkbEntEntQueryHandler::setLeftValidator(callsArgValidator);
  PkbEntEntQueryHandler::setRightValidator(callsArgValidator);
  PkbEntEntQueryHandler::setLeftTransformer(procArgTransformer);
}
