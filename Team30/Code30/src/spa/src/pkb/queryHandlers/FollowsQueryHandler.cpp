#include "FollowsQueryHandler.h"

FollowsQueryHandler::FollowsQueryHandler(FollowsStorage* storage,
                                         IStructureMappingProvider* provider,
                                         StmtPredicateFactory* factory)
    : store(storage), provider(provider), factory(factory) {}

QueryResultPtr<StmtValue, StmtValue> FollowsQueryHandler::query(
    StmtRef* arg1, StmtRef* arg2) const {
  if (arg1->isKnown()) {
    return store->query(arg1->getValue(), factory->getPredicate(arg2));
  } else if (arg2->isKnown()) {
    return store->query(factory->getPredicate(arg1), arg2->getValue());
  } else {
    return store->query(provider->getValuesOfType(arg1->getType()),
                        factory->getPredicate(arg2));
  }
}
