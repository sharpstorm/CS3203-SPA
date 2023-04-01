#include "CallsQueryHandler.h"

#include "ArgTransformers.h"
#include "ArgValidators.h"

CallsQueryHandler::CallsQueryHandler(PkbEntEntQueryInvoker *invoker,
                                     CallsStorage *storage)
    : PkbEntEntQueryHandler(invoker, storage) {
  PkbEntEntQueryHandler::setLeftValidator(callsArgValidator);
  PkbEntEntQueryHandler::setRightValidator(callsArgValidator);
  //  PkbEntEntQueryHandler::setLeftTransformer(procArgTransformer);
}
