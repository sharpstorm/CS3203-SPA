#include "ParentTQueryHandler.h"

#include <memory>

#include "ArgValidators.h"
using std::make_unique;

ParentTQueryHandler::ParentTQueryHandler(ParentTStorage *storage,
                                         IStructureMappingProvider *provider,
                                         StmtPredicateFactory *factory)
    : store(storage), provider(provider), factory(factory) {}

QueryResultPtr<StmtValue, StmtValue> ParentTQueryHandler::query(
    StmtRef *leftArg, StmtRef *rightArg) const {
  if (!parentLeftArgValidator(leftArg)) {
    return make_unique<QueryResult<StmtValue, StmtValue>>();
  }
  if (leftArg->isKnown() && rightArg->isKnown()) {
    return store->query(leftArg->getValue(), rightArg->getValue());
  } else if (leftArg->isKnown() && !rightArg->isKnown()) {
    return store->query({leftArg->getValue()},
                        provider->getValuesOfType(rightArg->getType()));
  } else if (rightArg->isKnown()) {
    return store->query(factory->getPredicate(leftArg), rightArg->getValue());
  } else {
    return store->query(provider->getValuesOfType(leftArg->getType()),
                        provider->getValuesOfType(rightArg->getType()));
  }
}
